declare const loris: any;

interface ApiResponse<T> {
    data: T,
    // Other fields like 'message', 'status', etc., can be added here
}

interface ApiError {
    message: string,
    code: number,
    // Additional error details can be added here
}

// XXX: IAPI might not actually be necessary
// interface IAPI<T> {
//   getAll(): Promise<T[]>,
//   getById(id: string): Promise<T>,
//   create(data: T): Promise<T>,
//   batchCreate(entities: T[]): Promise<T[]>,
//   update(id: string, data: T): Promise<T>,
//   batchUpdate(entities: T[]): Promise<T[]>,
//   streamData(setProgress: (progress: number) => void): Promise<T[]>,
//   handleError(response: Response): void;
// }


export default class BaseAPI<T> {
  protected baseUrl: string;

  constructor(baseUrl: string) {
    this.baseUrl = loris.BaseURL+'/api/v0.0.3'+baseUrl;
  }

  async getAll(): Promise<T[]> {
    return await BaseAPI.fetchJSON(this.baseUrl);
  }

  async getById(id: string): Promise<T> {
    return await BaseAPI.fetchJSON<T>(`${this.baseUrl}/${id}`);
  }

  async create(data: T): Promise<T> {
    return await BaseAPI.fetchJSON<T>(this.baseUrl, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(data),
    });
  }

  async batchCreate(entities: T[]): Promise<T[]> {
    return await BaseAPI.fetchJSON<T[]>(`${this.baseUrl}/batch-create`, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(entities),
    });
  }
  
  async update(id: string, data: T): Promise<T> {
    return await BaseAPI.fetchJSON<T>(`${this.baseUrl}/${id}`, {
      method: "PUT",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(data),
    });
  }
  
  async batchUpdate(entities: T[]): Promise<T[]> {
    return await BaseAPI.fetchJSON<T[]>(`${this.baseUrl}/batch-update`, {
      method: "PUT",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(entities),
    });
  }

  async fetchStream(setProgress: (progress: number) => void, signal: AbortSignal): Promise<T[]> {
    try {
      const response = await fetch(this.baseUrl, { signal });
      ErrorHandler.handleResponse(response, { url: this.baseUrl });

      if (!response.body) {
        throw new Error('No response body');
      }

      const reader = response.body.getReader();
      const contentLength = +response.headers.get('Content-Length') || 0;
      let receivedLength = 0;
      let chunks = [];

      while (true) {
        const { done, value } = await reader.read();
        if (done) break;

        chunks.push(value);
        receivedLength += value.length;
        const progress = Math.round((receivedLength / contentLength) * 100);
        setProgress(progress);
      }

      const chunksAll = new Uint8Array(receivedLength);
      let position = 0;
      for (let chunk of chunks) {
        chunksAll.set(chunk, position);
        position += chunk.length;
      }

      return JSON.parse(new TextDecoder("utf-8").decode(chunksAll));
    } catch (error) {
      ErrorHandler.handleError(error, { url: this.baseUrl} );
    }
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
