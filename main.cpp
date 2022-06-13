#include <iostream>
#include "cppzmq/zmq.hpp"
#include "client.h"

int main()
{
	Client client;
	client.connect("tcp://127.0.0.1:5555");
	client.sendBuffer("something.txt");
	return 0;
	void    *ctx = zmq_ctx_new ();
	assert ( ctx );
	/*                                             Create ZMQ_STREAM socket */
	void    *socket = zmq_socket ( ctx, ZMQ_STREAM );
	assert ( socket );

	int      rc = zmq_bind ( socket, "tcp://*:8080" );
	assert ( rc == 0 );

	/*                                            Data structure to hold the ZMQ_STREAM ID */
	uint8_t id [256];
	size_t  id_size = 256;

	/*                                            Data structure to hold the ZMQ_STREAM received data */
	uint8_t raw [256];
	size_t  raw_size = 256;

	while ( 1 ) {
		/*                                         Get HTTP request; ID frame and then request */
		id_size  = zmq_recv ( socket, id, 256, 0 );
		assert ( id_size >  0 );
		do {
				raw_size  = zmq_recv ( socket, raw, 256, 0 );
				assert ( raw_size >= 0 );
		} while (     raw_size == 256 );
		/*                                         Prepares the response */
		char http_response [] =
									"HTTP/1.0 200 OK\r\n"
									"Content-Type: text/plain\r\n"
									"\r\n"
									"Hello, World!";
		/*                                         Sends the ID frame followed by the response */
		zmq_send ( socket, id, id_size, ZMQ_SNDMORE );
		zmq_send ( socket, http_response, strlen ( http_response ), 0 );

		/*                                         Closes the connection by sending the ID frame followed by a zero response */
		zmq_send ( socket, id, id_size, ZMQ_SNDMORE );
		zmq_send ( socket, 0, 0, 0 );
	}
	zmq_close ( socket );
	zmq_ctx_destroy ( ctx );
	return 0;
}