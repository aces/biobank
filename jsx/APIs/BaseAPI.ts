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

export interface IAPI<T> {
    getAll(): Promise<T[]>,
    getById(id: string): Promise<T>,
    create(data: T): Promise<T>,
    batchCreate(entities: T[]): Promise<T[]>,
    update(id: string, data: T): Promise<T>,
    batchUpdate(entities: T[]): Promise<T[]>,
    // For streamData, you might need a specific return type depending on your implementation
    streamData(setProgress: (progress: number) => void): Promise<T[]>,
}

function handleHttpErrors(response: Response) {
  if (response.status === 404) {
    throw new Error("Resource not found");
  } else if (response.status === 401) {
    throw new Error("Unauthorized access");
  } else {
    throw new Error(`HTTP error! status: ${response.status}`);
  }
}

export class BaseAPI<T> implements IAPI<T> {
  protected baseUrl: string;

  constructor(baseUrl: string) {
    this.baseUrl = loris.BaseURL+baseUrl;
  }

  async getAll(): Promise<T[]> {
    try {
      const response = await fetch(`${this.baseUrl}`);
      
      if (!response.ok) {
        handleHttpErrors(response);
      }
  
      return response.json();
    } catch (error) {
      console.error("Error in getAll:", error);
      throw new Error("An error occurred when fetching data");
    }
  }

  async getById(id: string): Promise<T> {
    try {
      const response = await fetch(`${this.baseUrl}/${id}`);

      if (!response.ok) {
        handleHttpErrors(response);
      }

      return response.json();
    } catch (error) {
      console.error(`Error in getById for id ${id}:`, error);
      throw new Error(`An error occurred when fetching data for id ${id}`);
    }
  }

  async create(data: T): Promise<T> {
    try {
      const response = await fetch(`${this.baseUrl}`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      });

      if (!response.ok) {
        handleHttpErrors(response);
      }

      return response.json();
    } catch (error) {
      console.error("Error in create:", error);
      throw new Error("An error occurred when creating data");
    }
  }

  async batchCreate(entities: T[]): Promise<T[]> {
    try {
      const response = await fetch(`${this.baseUrl}/batch-create`, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(entities),
      });

      if (!response.ok) {
        handleHttpErrors(response);
      }

      return response.json();
    } catch (error) {
      console.error("Error in batchCreate:", error);
      throw new Error("An error occurred when batch creating data");
    }
  }

  async update(id: string, data: T): Promise<T> {
    try {
      const response = await fetch(`${this.baseUrl}/${id}`, {
        method: "PUT",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      });

      if (!response.ok) {
        handleHttpErrors(response);
      }

      return response.json();
    } catch (error) {
      console.error(`Error in update for id ${id}:`, error);
      throw new Error(`An error occurred when updating data for id ${id}`);
    }
  }

  async batchUpdate(entities: T[]): Promise<T[]> {
    try {
      const response = await fetch(`${this.baseUrl}/batch-update`, {
        method: "PUT",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify(entities),
      });

      if (!response.ok) {
        handleHttpErrors(response);
      }

      return response.json();
    } catch (error) {
      console.error("Error in batchUpdate:", error);
      throw new Error("An error occurred when batch updating data");
    }
  }

  // async delete(id: string): Promise<boolean> {
  //   const response: AxiosResponse<void> = await axios.delete(`${this.baseUrl}/${id}`);
  //   return response.status === 204;
  // }

  async streamData(setProgress: (progress: number) => void): Promise<T[]> {
    const response = await fetch(this.baseUrl, { credentials: 'same-origin', method: 'GET' });
    if (!response.body) {
      throw new Error('No response body');
    }

    const reader = response.body.getReader();
    const contentLength = +response.headers.get('Content-Length') || 0;
    let receivedLength = 0; // received that many bytes at the moment
    let chunks = []; // array of received binary chunks (comprises the body)
    
    while (true) {
      const { done, value } = await reader.read();

      if (done) {
        break;
      }

      chunks.push(value);
      receivedLength += value.length;

      // Update progress
      setProgress(Math.round((receivedLength / contentLength) * 100));
    }

    // Combine chunks into single Uint8Array
    const chunksAll = new Uint8Array(receivedLength);
    let position = 0;
    for (let chunk of chunks) {
      chunksAll.set(chunk, position);
      position += chunk.length;
    }

    // Decode into a string
    const result = new TextDecoder("utf-8").decode(chunksAll);

    // Parse the result
    return JSON.parse(result);
  }
}

export default BaseAPI;
