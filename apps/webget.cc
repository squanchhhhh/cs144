#include "../util/socket.hh"
#include "../util/address.hh"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <span>
#include <string>
#include <sys/socket.h>

using namespace std;

void get_URL( const string& host, const string& path )
{
  char request[256]; // Assuming a reasonable buffer size for the request
  std::sprintf(request, "GET http://%s%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", host.c_str(), path.c_str(), host.c_str());
  auto client = TCPSocket();
  // auto address = Address(host,80);
  // Define the proxy settings
  const string proxy_host = "192.168.0.100"; 
  const string proxy_port = "7890"; 
  addrinfo hints;
  std::memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    // Connect to the proxy server
  auto proxy_address = Address(proxy_host, proxy_port);
  client.connect(proxy_address);

  client.write(request);
  cout<<request;
  client.shutdown(SHUT_WR);
  // Read the response from the proxy server
  string response;
  while (!client.eof()) {
      string temp;
      client.read(temp);
      response += temp;
  }
  cout<<response<<endl;
  client.close();
  // cerr << "Function called: get_URL(" << host << ", " << path << ")\n";
  // cerr << "Warning: get_URL() has not been implemented yet.\n";
}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
