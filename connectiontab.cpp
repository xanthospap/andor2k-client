#include "connectiontab.h"
#include "helmos-andor2k/cpp_socket.hpp"
#include <QRegularExpressionValidator>
#include <QMessageBox>

using andor2k::ClientSocket;
using andor2k::Socket;

ConnectionTab::ConnectionTab(ClientSocket *&csocket, QWidget *parent)
    : QWidget(parent) {
  printf("[DEBUG][ANDOR2K::client::%15s] Constructing ConnectionTab\n", __func__);
  
  createGui();
  setLayout(v_main_layout);
  csock = &csocket;
  connect(m_connect_button, SIGNAL(clicked()), this, SLOT(sock_connect()));
  connect(m_defaults_button, SIGNAL(clicked()), this, SLOT(reset_defaults()));
  connect(m_disconnect_button, SIGNAL(clicked()), this, SLOT(disconnect()));
  
  printf("[DEBUG][ANDOR2K::client::%15s] ConnectionTab Socket at %p -> %p", __func__, &csock, csock);
  if (*csock)
    printf(" -> %p\n", *csock);
  else
    printf(" -> nowhere!\n");
  printf("[DEBUG][ANDOR2K::client::%15s] Finished constructing ConnectionTab\n", __func__);
}

void ConnectionTab::createGui() {
  m_hostname_ledit = new QLineEdit;
  m_hostname_ledit->setPlaceholderText("Hostname IP");
  m_hostname_ledit->setText("localhost");
  m_hostname_ledit->setToolTip("Hostname where ANDOR2K daemon is running");

  m_port_ledit = new QLineEdit;
  m_port_ledit->setPlaceholderText("Listening Port On Host");
  m_port_ledit->setText("8080");
  QRegularExpression rx(
      "\\d{3,5}"); /* only allow integers in range [100-99999] */
  QValidator *validator = new QRegularExpressionValidator(rx, this);
  m_port_ledit->setValidator(validator);
  m_port_ledit->setToolTip("Port to connect on for ANDOR2K daemon");

  m_connect_button = new QPushButton("Connect");
  m_connect_button->setEnabled(true);

  m_disconnect_button = new QPushButton("Disconnect", this);
  m_disconnect_button->setEnabled(false);

  m_defaults_button = new QPushButton("Reset Defaults");
  m_defaults_button->setEnabled(true);

  g_edits_layout = new QGridLayout;
  g_edits_layout->addWidget(new QLabel(tr("Host:")), 0, 0);
  g_edits_layout->addWidget(m_hostname_ledit, 0, 1);
  g_edits_layout->addWidget(new QLabel(tr("Port:")), 1, 0);
  g_edits_layout->addWidget(m_port_ledit, 1, 1);

  h_button_layout = new QHBoxLayout;
  h_button_layout->addWidget(m_connect_button);
  h_button_layout->addWidget(m_disconnect_button);
  h_button_layout->addWidget(m_defaults_button);

  v_main_layout = new QVBoxLayout;
  v_main_layout->addLayout(g_edits_layout);
  v_main_layout->addLayout(h_button_layout);
}

void ConnectionTab::sock_connect() {
  if (*csock) {
    QMessageBox msbox(QMessageBox::Critical, "Connection Error", "Already Connected!");
    msbox.exec();
    return;
  }

  std::string host = m_hostname_ledit->text().toStdString();
  try {
    *csock = new ClientSocket(host.c_str(), m_port_ledit->text().toInt());
  } catch (std::exception &e) {
    *csock = nullptr;
    QMessageBox messageBox(QMessageBox::Critical, "Connection Error", "Could not connect to daemon!");
    messageBox.exec();
    return;
  }
  m_connect_button->setEnabled(false);
  m_disconnect_button->setEnabled(true);
  
  printf("[DEBUG][ANDOR2K::client::%15s] Socket points to: ", __func__);
  if (*csock) {
    printf("%p -> %p\n", csock, *csock);
  } else {
    printf(" nowhere!\n");
  }
}

void ConnectionTab::reset_defaults() {
  m_hostname_ledit->setText("localhost");
  m_port_ledit->setText("8080");
  m_defaults_button->setEnabled(false);
}

void ConnectionTab::disconnect() {
  (*csock)->close_socket();
  *csock = nullptr;
  m_connect_button->setEnabled(true);
  m_disconnect_button->setEnabled(false);
}
