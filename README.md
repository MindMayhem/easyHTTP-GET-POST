# easyHTTP-GET-POST
Single-file library for executing GET and POST requests with the ability to transfer data to POST

# Using GET
***HTTP_GET(domain, /requesturl)*** - will return respone of GET request in std::string format without headers

Example - *HTTP_GET("ip-api.com","/json")*

return: {"status":"success", blablabla}

# Using POST
***HTTP_POST(domain, /requesturl, data)*** - will return respone of GET request in std::string format without headers

Example - *HTTP_POST("ip-api.com","/json", data)*

Great solution for sending POST requsts like files or any other shit u want

Feel free to use it ^_^
