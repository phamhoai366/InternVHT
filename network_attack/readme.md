# 1. CA root key and certificate generation process:
Generate root key
$ openssl genrsa -des3 -out root.key
Generate root certificate
$ openssl req -new -key root.key -out root.csr
Root certificate self-signed
$ openssl x509 -req -days 3650 -sha1 -extensions v3_ca -signkey root.key -in root.csr -out root.crt


# 2.Server key and certificate generation process:
Generate server key
$ openssl genrsa -des3 -out server.key 2048
Generate server certificate
$ openssl req -new -key server.key -out server.csr
Use the root certificate and key to sign the service terminal certificate
$ openssl x509 -req -days 3650 -sha1 -extensions v3_req -CA root.crt -CAkey root.key -CAcreateserial -in server.csr -out server.crt


# 3. Client key and certificate generation process:
Generate client key
$ openssl genrsa -des3 -out client.key 2048
Generate client certificate
$ openssl req -new -key client.key -out client.csr
Use root certificate and key to sign client sub-certificate
$ openssl x509 -req -days 3650 -sha1 -extensions v3_req -CA root.crt -CAkey root.key -CAcreateserial -in client.csr -out client.crt

# 4. Merge certificate and key to generate PFX file:
PFX format for client installation
$ openssl pkcs12 -export -in client.crt -inkey client.key -out client.pfx
