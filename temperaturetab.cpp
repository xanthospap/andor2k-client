#include "temperaturetab.h"
#include <QApplication>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <cstring>

using andor2k::ClientSocket;
using andor2k::Socket;

TemperatureTab::TemperatureTab(ClientSocket *&csocket, char *sock_buffer,
                               QWidget *parent)
    : QWidget(parent) {
  printf("[DEBUG][ANDOR2K::client::%15s] Constructing TemperatureTab\n",
         __func__);
  createGui();
  setLayout(m_layout);
  csock = &csocket;
  buffer = sock_buffer;
  connect(m_set_button, SIGNAL(clicked()), this, SLOT(set_temperature()));

  printf("[DEBUG][ANDOR2K::client::%15s] TemperatureTab Socket at %p -> %p",
         __func__, &csock, csock);
  if (*csock)
    printf(" -> %p\n", *csock);
  else
    printf(" -> nowhere!\n");
  printf(
      "[DEBUG][ANDOR2K::client::%15s] Finished constructing TemperatureTab\n",
      __func__);
}

void TemperatureTab::createGui() {

  m_set_temp_ledit = new QLineEdit;
  QRegularExpression rx(
      "[+-]?\\d{1,3}"); /* only allow integers in range [-999, 999] */
  QValidator *validator = new QRegularExpressionValidator(rx, this);
  m_set_temp_ledit->setValidator(validator);
  m_set_temp_ledit->setToolTip(
      "Set ANDOR2K temperature; range depends on camera");

  m_show_temp_ledit = new QLineEdit;

  m_label = new QLabel;
  m_label->setFrameStyle(QFrame::Box | QFrame::Plain);

  m_set_button = new QPushButton("Set");

  m_layout = new QGridLayout;
  m_layout->addWidget(new QLabel(tr("Current Temperature (C):")), 0, 0);
  m_layout->addWidget(m_show_temp_ledit, 0, 1);
  m_layout->addWidget(new QLabel(tr("Set Temperature to (C):")), 1, 0);
  m_layout->addWidget(m_set_temp_ledit, 1, 1);
  m_layout->addWidget(m_set_button, 2, 0);
  m_layout->setSizeConstraint(QLayout::SetFixedSize);
}

void TemperatureTab::set_temperature() {

  printf("[DEBUG][ANDOR2K::client::%15s] Socket points to: ", __func__);
  if (*csock) {
    printf("%p -> %p\n", csock, *csock);
  } else {
    printf(" nowhere!\n");
  }

  /* check that the connection/socket is alive */
  if (*csock == nullptr) {
    QMessageBox msbox(QMessageBox::Critical, "Connection Error",
                      "Cannot send command to daemon! Need to connect first");
    msbox.exec();
    return;
  }

  /* check if user has passed in a temperature value */
  if (m_set_temp_ledit->text().isEmpty()) {
    QMessageBox msbox(QMessageBox::Critical, "Empty Value",
                      "Must provide a valid temperature value!");
    msbox.exec();
    return;
  }

  /* fill parent buffer with the cmmand: "settemp TemperatureValue" */
  auto tval = m_set_temp_ledit->text().toStdString();
  std::memset(buffer, '\0', 1024);
  std::strcpy(buffer, "settemp ");
  std::strcpy(buffer + std::strlen(buffer), tval.c_str());

  /* send command to deamon */
  printf("[DEBUG][ANDOR2K::client::%15s] sending command: \"%s\"\n", __func__,
         buffer);
  (*csock)->send(buffer);
}
