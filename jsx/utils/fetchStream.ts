async function fetchStream(url, handleRow, setData, setProgress, signal) {
    const response = await fetch(url, {
        method: 'GET',
        credentials: 'same-origin',
        signal: signal,
    });

    const reader = response.body.getReader();
    const contentLength = response.headers.get('Content-Length');

    let receivedLength = 0; // The total number of bytes received so far
    let chunks = []; // Array of received binary chunks

    while (true) {
        const { done, value } = await reader.read();

        if (done) {
            break; // The stream has been fully read
        }

        chunks.push(value);
        receivedLength += value.length;

        // If the total size is known, calculate the progress
        if (contentLength) {
            const progress = (receivedLength / contentLength) * 100;
            setProgress(progress);
        } else {
            // If content length is unknown, you can provide feedback based on received length
            console.log(`Received ${receivedLength} bytes so far`);
        }
    }

    // Combine all chunks into a single Uint8Array
    let chunksAll = new Uint8Array(receivedLength);
    let position = 0;
    for (let chunk of chunks) {
        chunksAll.set(chunk, position);
        position += chunk.length;
    }

    // Convert the binary data to a text or JSON object (depends on your use case)
    const resultText = new TextDecoder("utf-8").decode(chunksAll);
    const data = JSON.parse(resultText); // Assuming JSON data; adjust as needed

    // Handle each row if the data is an array
    if (Array.isArray(data)) {
        data.forEach(row => handleRow(row));
        setData(data);
    } else {
        // Handle different data formats if necessary
        console.error('Unexpected data format:', data);
    }

    setProgress(100); // Ensure progress is set to 100% when done
}

