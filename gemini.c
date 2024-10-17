#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#define API_URL "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-latest:generateContent?key=<yourapikeyhere>"

size_t WriteCallback(void *contents, size_t size, size_t nmemb, char *userp) {
    strncat(userp, contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input phrase>\n", argv[0]);
        return 1;
    }

    char input[1024] = "";
    for (int i = 1; i < argc; i++) {
        strcat(input, argv[i]);
        if (i < argc - 1) {
            strcat(input, " ");
        }
    }
    cJSON *json = cJSON_CreateObject();
    cJSON *contents = cJSON_CreateArray();
    cJSON *content_item = cJSON_CreateObject();
    cJSON *parts = cJSON_CreateArray();
    cJSON *textPart = cJSON_CreateObject();

    cJSON_AddItemToArray(parts, textPart);
    cJSON_AddStringToObject(textPart, "text", input);
    cJSON_AddItemToObject(content_item, "parts", parts);
    cJSON_AddItemToArray(contents, content_item);
    cJSON_AddItemToObject(json, "contents", contents);

    char *json_payload = cJSON_Print(json);
    CURL *curl;
    CURLcode res;
    char response[4096] = "";

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            cJSON_Delete(json);
            free(json_payload);
            curl_easy_cleanup(curl);
            return 1;
        }

        cJSON *json_response = cJSON_Parse(response);
        if (json_response != NULL) {
            cJSON *candidates = cJSON_GetObjectItemCaseSensitive(json_response, "candidates");
            if (candidates && cJSON_IsArray(candidates) && cJSON_GetArraySize(candidates) > 0) {
                cJSON *first_candidate = cJSON_GetArrayItem(candidates, 0);
                cJSON *content = cJSON_GetObjectItemCaseSensitive(first_candidate, "content");
                if (content) {
                    cJSON *parts = cJSON_GetObjectItemCaseSensitive(content, "parts");
                    if (parts && cJSON_IsArray(parts) && cJSON_GetArraySize(parts) > 0) {
                        cJSON *text_part = cJSON_GetArrayItem(parts, 0);
                        printf("%s\n", cJSON_GetObjectItemCaseSensitive(text_part, "text")->valuestring);
                    } else {
                        fprintf(stderr, "No parts found in content.\n");
                    }
                } else {
                    fprintf(stderr, "No content found in candidate.\n");
                }
            } else {
                fprintf(stderr, "No candidates found in response.\n");
            }
            cJSON_Delete(json_response);
        } else {
            fprintf(stderr, "Error parsing JSON response: %s\n", cJSON_GetErrorPtr());
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Failed to initialize cURL.\n");
    }

    free(json_payload);
    cJSON_Delete(json);

    return 0;
}
