# restless++
Embeddable RESTful Web Service Framework for C++ Applications

This project is built around Civet's web server (https://github.com/sunsetbrew/civetweb). 
It was simplified and modified to provide an easy way to embed RESTful web services inside a new or an existing C++ application.

The framework contains:
 - A Web Server (Civet)
 - A C++ interface to register pattern based path handlers (e.g. /{some}/{value}?{queryA} where each pattern becomes a parameter to a handler)
 - An lightweight introspection framework which parses simple C++ files and generates class metadata.
 - A fast JSON serializer and deserializer based on the generated metadata. Serialization and deserialization for types is generated at compile time using template programming.

