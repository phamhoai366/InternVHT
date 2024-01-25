# network attack
sudo ./main <iface ...> <vitcim's IP> <gateway's IP> 

content type:

20 (hex 0x14): Alert
21 (hex 0x15): Handshake
22 (hex 0x16): Application Data
23 (hex 0x17): Heartbeat

handshake type: 

0x00: HelloRequest
0x01: ClientHello
0x02: ServerHello
0x0B: Certificate
0x0C: ServerKeyExchange
0x0D: CertificateRequest
0x0E: ServerHelloDone
0x0F: CertificateVerify
0x10: ClientKeyExchange
0x14: Finished


