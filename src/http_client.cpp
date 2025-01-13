#include "http_client.h"


// Callback function to handle data received
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

std::string extractHash(const std::string& jsonResponse) {
    // size_t hashStart = jsonResponse.find("\"hash\":\"");
    
    size_t hashStart = jsonResponse.find("\"flight_id\":\"");


    // if (hashStart == std::string::npos) {
    //     return "Hash not found!";
    // }

    // // Start position after "hash":" (7 characters long)
    // hashStart += 8;
    // size_t hashEnd = jsonResponse.find("\"", hashStart);
    // if (hashEnd == std::string::npos) {
    //     return "Invalid JSON format!";
    // }

    // // Extract and return the hash
    // return jsonResponse.substr(hashStart, hashEnd - hashStart);
    const std::string key = "\"flight_id\":\"";
    size_t startPos = jsonResponse.find(key);
    if (startPos == std::string::npos) {
        return "Flight ID not found!";
    }

    // Position after the "flight_id":" key
    startPos += key.length();
    size_t endPos = jsonResponse.find("\"", startPos);
    if (endPos == std::string::npos) {
        return "Invalid JSON format!";
    }

    return jsonResponse.substr(startPos, endPos - startPos);
}

std::string getFlightID()
{
    CURL* curl;
    CURLcode res;
    std::string response;

    std::string hash;

    curl = curl_easy_init();
    if (curl) {
        // Set URL for the GET request
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:10000/my-flight-id");

        // Set write function to handle response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        // Set buffer to store the response
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            hash = "";
        }
        
        else
        {
            std::cout << "Response: " << response << std::endl;
            hash = extractHash(response);
            std::cout << "Extracted Hash: " << hash << std::endl;
            // log_info("Extracted Hash: %s\n", hash);
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    return hash;

}

// int main()
// {
    
//     getFlightID();
//     return 0;
// }
