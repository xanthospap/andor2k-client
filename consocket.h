#ifndef CONSOCKET_H
#define CONSOCKET_H

#include "helmos-andor2k/cpp_socket.hpp"

class ConSocket {
public:
  ConSocket();
  andor2k::ClientSocket *csock = nullptr;
};

#endif // CONSOCKET_H
