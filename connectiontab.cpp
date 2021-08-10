#include "connectiontab.h"
#include "settempthread.h"
#include "helmos-andor2k/cpp_socket.hpp"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QGroupBox>
#include <cstring>
#include <unistd.h>
#include <QThread>

using andor2k::ClientSocket;
using andor2k::Socket;

ConnectionTab::ConnectionTab(ClientSocket *&csocket, QWidget *parent)
    : QWidget(parent) {
  printf("[DEBUG][ANDOR2K::client::%15s] Constructing ConnectionTab\n",
         __func__);

  createGui();
  setLayout(v_main_layout);
  csock = &csocket;
  connect(m_connect_button, SIGNAL(clicked()), this, SLOT(sock_connect()));
  connect(m_defaults_button, SIGNAL(clicked()), this, SLOT(reset_defaults()));
  connect(m_disconnect_button, SIGNAL(clicked()), this, SLOT(disconnect()));
  connect(m_shutdown_button, SIGNAL(clicked()), this, SLOT(shutdown_daemon()));

  printf("[DEBUG][ANDOR2K::client::%15s] ConnectionTab Socket at %p -> %p",
         __func__, &csock, csock);
  if (*csock)
    printf(" -> %p\n", *csock);
  else
    printf(" -> nowhere!\n");
  printf("[DEBUG][ANDOR2K::client::%15s] Finished constructing ConnectionTab\n",
         __func__);
}

void ConnectionTab::createGui() {
  m_hostname_ledit = new QLineEdit;
  //m_hostname_ledit->setPlaceholderText("Hostname IP");
  m_hostname_ledit->setText("localhost");
  m_hostname_ledit->setToolTip("Hostname where ANDOR2K daemon is running; note that for local connections you can use either \"127.0.0.1\" or \"localhost\"");

  m_port_ledit = new QLineEdit;
  // m_port_ledit->setPlaceholderText("Listening Port On Host");
  m_port_ledit->setText("8080");
  QRegularExpression rx(
      "\\d{3,5}"); /* only allow integers in range [100-99999] */
  QValidator *validator = new QRegularExpressionValidator(rx, this);
  m_port_ledit->setValidator(validator);
  m_port_ledit->setToolTip("Port to connect on for ANDOR2K daemon");

  m_init_temp = new QLineEdit;
  // m_init_temp->setPlaceholderText("Set initial target temperature (dec C)");
  m_init_temp->setText("-90");
  QRegularExpression rtx("[+-]?[0-9]{1,3}");
  QValidator *tvalidator = new QRegularExpressionValidator(rtx, this);
  m_init_temp->setValidator(tvalidator);
  m_init_temp->setToolTip("Target temperature (in Celsius) fot the camera at startup");

  m_observer = new QLineEdit;
  // get user name
  char usern[32];
  QString user;
  if (getlogin_r(usern, 32)) {
    user = "andor2k";
  } else {
    user = QString(usern);
  }
  m_observer->setText(user);
  m_observer->setToolTip("Observer name; this name will be written to the FITS header, if exposures are to be taken");

  m_temp_c = new QLineEdit;
  m_temp_c->setReadOnly(true);
  m_con_status = new QLineEdit;
  m_con_status->setText("not connected");
  m_con_status->setReadOnly(true);
  m_server_info = new QLineEdit;
  m_server_info->setReadOnly(true);
  m_time = new QLineEdit;
  m_time->setReadOnly(true);

  m_connect_button = new QPushButton("Connect");
  m_connect_button->setEnabled(true);

  m_disconnect_button = new QPushButton("Disconnect", this);
  m_disconnect_button->setEnabled(false);

  m_defaults_button = new QPushButton("Reset Defaults");
  m_defaults_button->setEnabled(true);

  m_shutdown_button = new QPushButton("Shutdown Daemon");
  m_shutdown_button->setEnabled(false);
  
  // group connection options
  QGroupBox *con_gbox = new QGroupBox(tr("Connection Options"));
  QGridLayout *con_layout = new QGridLayout;
  con_layout->addWidget(new QLabel(tr("Host:")), 0, 0);
  con_layout->addWidget(m_hostname_ledit, 0, 1);
  con_layout->addWidget(new QLabel(tr("Port:")), 1, 0);
  con_layout->addWidget(m_port_ledit, 1, 1);
  con_gbox->setLayout(con_layout);
  
  // group startup options
  QGroupBox *init_gbox = new QGroupBox(tr("Initialization Options"));
  QGridLayout *init_layout = new QGridLayout;
  init_layout->addWidget(new QLabel(tr("Target Temperature st StartUp")), 0, 0);
  init_layout->addWidget(m_init_temp, 0, 1);
  init_layout->addWidget(new QLabel(tr("deg C.")), 0, 2);
  init_layout->addWidget(new QLabel(tr("Observer Name")),1,0);
  init_layout->addWidget(m_observer, 1, 1, 1, 2);
  init_gbox->setLayout(init_layout);

  // group buttons
  h_button_layout = new QHBoxLayout;
  h_button_layout->addWidget(m_connect_button);
  h_button_layout->addWidget(m_disconnect_button);
  h_button_layout->addWidget(m_defaults_button);

  // group server response status
  QGroupBox *server_gbox = new QGroupBox(tr("Server Status"));
  QGridLayout *server_layout = new QGridLayout;
  server_layout->addWidget(new QLabel(tr("Connection Status")), 0,0);
  server_layout->addWidget(m_con_status,0,1);
  server_layout->addWidget(new QLabel(tr("ANDOR2K Temperature (deg C)")), 1,0);
  server_layout->addWidget(m_temp_c,1,1);
  server_layout->addWidget(new QLabel(tr("ANDOR2K Status")), 2,0);
  server_layout->addWidget(m_server_info,2,1);
  server_layout->addWidget(new QLabel(tr("Last Response Time")), 3,0);
  server_layout->addWidget(m_time,3,1);
  server_gbox->setLayout(server_layout);

  QVBoxLayout *v_options_layout = new QVBoxLayout;
  v_options_layout = new QVBoxLayout;
  v_options_layout->addWidget(con_gbox);
  v_options_layout->addWidget(init_gbox);
  v_options_layout->addLayout(h_button_layout);
  
  QHBoxLayout *hb_layout = new QHBoxLayout;
  hb_layout->addLayout(v_options_layout);
  hb_layout->addWidget(server_gbox);

  v_main_layout = new QVBoxLayout;
  v_main_layout->addLayout(hb_layout);
  v_main_layout->addWidget(m_shutdown_button);
}

void ConnectionTab::send_settemp() {
  if (*csock == nullptr) {
    QMessageBox msbox(QMessageBox::Critical, "Connection Error",
                      "Cannot send command to daemon! Need to connect first");
    msbox.exec();
    return;
  }

  std::memset(buffer, 0, 1024);
  int target_temp = m_init_temp->text().toInt();
  sprintf(buffer, "settemp %d", target_temp);
  (*csock)->send(buffer);

  setTempThread *tthread = new setTempThread(*csock);
  connect(tthread, &setTempThread::resultReady, this, &ConnectionTab::serverJobDone);
  connect(tthread, &setTempThread::newResponseReady, this, &ConnectionTab::serverJobUpdate);
  connect(tthread, &setTempThread::finished, tthread, &QObject::deleteLater);
  tthread->start();
  

  /*
  QStringList list;
  QString val;
  // keep updating menu while server response
  bool server_done = false;
  while (!server_done) {
    std::memset(buffer, 0, 1024);
    if ( (*csock)->recv(buffer, 1024) < 0 ) {
      printf("--- ERROR failed to receiver server message\n");
    }
    printf("<----- Server status: [%s]\n", buffer);
    
    QString response (buffer);
    list = split_command(buffer);
    m_temp_c->setText(get_val("temp", list));
    printf("\tset m_temp_c to %s\n", get_val("temp", list).toStdString().c_str());
    m_server_info->setText(get_val("info", list));
    printf("\tset m_server_info to %s\n", get_val("info", list).toStdString().c_str());
    m_time->setText(get_val("time", list));
    printf("\tset m_time to %s\n", get_val("time", list).toStdString().c_str());
    
    if (!(*buffer) || !std::strncmp(buffer, "done", 4)) {
      server_done = true;
      printf("server signaled work done!\n");
    }
  }
  */
  
  
  return;
}

void ConnectionTab::serverJobUpdate(const QString& response) {
  m_server_info->setText(response);
}

void ConnectionTab::serverJobDone() {
  m_connect_button->setEnabled(false);
  m_disconnect_button->setEnabled(true);
  m_shutdown_button->setEnabled(true);
}

void ConnectionTab::sock_connect() {
  if (*csock) {
    QMessageBox msbox(QMessageBox::Critical, "Connection Error",
                      "Already Connected!");
    msbox.exec();
    return;
  }

  std::string host = m_hostname_ledit->text().toStdString();
  try {
    *csock = new ClientSocket(host.c_str(), m_port_ledit->text().toInt());
  } catch (std::exception &e) {
    *csock = nullptr;
    QMessageBox messageBox(QMessageBox::Critical, "Connection Error",
                           "Could not connect to daemon!");
    messageBox.exec();
    return;
  }
  m_connect_button->setEnabled(false);
  m_disconnect_button->setEnabled(true);
  m_shutdown_button->setEnabled(true);
  m_con_status->setText("connected");

  send_settemp();

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
  m_init_temp->setText("-90");
  m_defaults_button->setEnabled(false);
}

void ConnectionTab::disconnect() {
  (*csock)->close_socket();
  delete (*csock);
  *csock = nullptr;
  m_connect_button->setEnabled(true);
  m_disconnect_button->setEnabled(false);
  m_shutdown_button->setEnabled(false);
  m_con_status->setText("not connected");
}

void ConnectionTab::shutdown_daemon() {
  char buffer[1024];
  std::memset(buffer, 0, 1024);
  std::strcpy(buffer, "shutdown");
  (*csock)->send(buffer);
  disconnect();
  m_con_status->setText("not connected");
}
