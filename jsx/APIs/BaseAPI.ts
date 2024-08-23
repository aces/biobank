declare const loris: any;
import Query, { QueryParam } from './Query';
import fetchDataStream from 'jslib/fetchDataStream';

interface ApiResponse<T> {
    data: T,
    // Other fields like 'message', 'status', etc., can be added here
}

interface ApiError {
    message: string,
    code: number,
    // Additional error details can be added here
}

export default class BaseAPI<T> {
  protected baseUrl: string;
  protected subEndpoint: string;

  constructor(baseUrl: string) {
    this.baseUrl = loris.BaseURL+'/biobank/'+baseUrl;
  }

  setSubEndpoint(subEndpoint: string): this {
    this.subEndpoint = subEndpoint;
    return this;
  }

  async get<U = T>(query?: Query): Promise<U[]> {
    const path = this.subEndpoint ? `${this.baseUrl}/${this.subEndpoint}` : this.baseUrl;
    const queryString = query ? query.build() : '';
    const url = queryString ? `${path}?${queryString}` : path;
    return BaseAPI.fetchJSON<U[]>(url);
  }

  async getLabels(...params: QueryParam[]): Promise<string[]> {
    const query = new Query();
    params.forEach(param => query.addParam(param));
    return this.get<string>(query.addField('label'));
  }

  async getById(id: string): Promise<T> {
    return BaseAPI.fetchJSON<T>(`${this.baseUrl}/${id}`);
  }


  async create(data: T): Promise<T> {
    return BaseAPI.fetchJSON<T>(this.baseUrl, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(data),
    });
  }

  async batchCreate(entities: T[]): Promise<T[]> {
    return BaseAPI.fetchJSON<T[]>(`${this.baseUrl}/batch-create`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(entities),
    });
  }
  
  async update(id: string, data: T): Promise<T> {
    return BaseAPI.fetchJSON<T>(`${this.baseUrl}/${id}`, {
      method: "PUT",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(data),
    });
  }
  
  async batchUpdate(entities: T[]): Promise<T[]> {
    return BaseAPI.fetchJSON<T[]>(`${this.baseUrl}/batch-update`, {
      method: "PUT",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(entities),
    });
  }

  static async fetchJSON<T>(url: string, options?: RequestInit): Promise<T> {
    try {
      const response = await fetch(url, { ...options });
      ErrorHandler.handleResponse(response, { url, options });
      const data = await response.json();
      return data;
    } catch (error) {
      ErrorHandler.handleError(error, { url, options} );
    }
  }

  async fetchStream(
    addEntity: (entity: T) => void,
    setProgress: (progress: number) => void,
    signal: AbortSignal
  ): Promise<void> {
    const url = new URL(this.baseUrl);
    url.searchParams.append('format', 'json');
  
    try {
      await this.streamData(url.toString(), addEntity, setProgress, signal);
    } catch (error) {
      if (signal.aborted) {
        console.log('Fetch aborted');
      } else {
        throw error;
      }
    }
  }

  async streamData(
    dataURL: string,
    addEntity: (entity: T) => void,
    setProgress: (progress: number) => void,
    signal: AbortSignal
  ): Promise<void> {
    const response = await fetch(dataURL, {
      method: 'GET',
      credentials: 'same-origin',
      signal,
    });
  
    const reader = response.body.getReader();
    const utf8Decoder = new TextDecoder('utf-8');
    let remainder = ''; // For JSON parsing
    let processedSize = 0;
    const contentLength = +response.headers.get('Content-Length') || 0;
    console.log('Content Length: '+contentLength);
  
    while (true) {
      const { done, value } = await reader.read();
  
      if (done) {
        if (remainder.trim()) {
          try {
            console.log(remainder);
            addEntity(JSON.parse(remainder));
          } catch (e) {
            console.error("Failed to parse final JSON object:", e);
          }
        }
        break;
      }
  
      const chunk = utf8Decoder.decode(value, { stream: true });
      remainder += chunk;
  
      let boundary = remainder.indexOf('\n'); // Assuming newline-delimited JSON objects
      while (boundary !== -1) {
        const jsonStr = remainder.slice(0, boundary);
        remainder = remainder.slice(boundary + 1);
  
        try {
          addEntity(JSON.parse(jsonStr));
        } catch (e) {
          console.error("Failed to parse JSON object:", e);
        }
  
        boundary = remainder.indexOf('\n');
      }
  
      processedSize += value.length;
      if (setProgress && contentLength > 0) {
        setProgress(Math.min((processedSize / contentLength) * 100, 100));
      }
    }
  
    setProgress(100); // Ensure progress is set to 100% on completion
  }
}

class ErrorHandler {
  static logDetailedError(error: Error, context: { url: string; options?: RequestInit }) {
    console.error(`Error requesting ${context.url} with options
                  ${JSON.stringify(context.options)}: `, error);
  }

  static handleResponse(
    response: Response,
    context: {
      url: string;
      options?: RequestInit 
    }
  ) {
    if (!response.ok) {
      this.handleError(new Error(`HTTP error! Status: ${response.status}`), context);
    }
    return response;
  }

  static handleError(
    error: any,
    context: { url: string, options?: RequestInit },
  ) {
    if (error instanceof DOMException && error.name === 'AbortError') {
      // Log for informational purposes, but treat it as a non-critical error
      console.info("Request was aborted by the client:", error);
      return; // Exit early, do not throw further, as this is an expected scenario in abort cases
    }

    if (error instanceof Response && !error.ok) {
      console.error(`HTTP error! Status: ${error.status}`);
    } else {
      this.logDetailedError(error, context); // Log detailed information about the error
      throw new Error(`API Error: ${error.message || "An unknown error occurred"}`);
    }
  }
}
