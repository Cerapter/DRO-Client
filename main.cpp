#include "aoapplication.h"

#include "datatypes.h"
#include "networkmanager.h"
#include "lobby.h"
#include "courtroom.h"
#include <QPluginLoader>
#include <QDebug>

int main(int argc, char *argv[])
{
#if QT_VERSION > QT_VERSION_CHECK(5, 6, 0)
  // High-DPI support is for Qt version >=5.6.
  // However, many Linux distros still haven't brought their stable/LTS
  // packages up to Qt 5.6, so this is conditional.
  AOApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

  QPluginLoader apng("imageformats/qapng.dll");
  if (!apng.load())
  {
    qDebug() << apng.errorString();
    exit(1);
  }

  AOApplication main_app(argc, argv);
  main_app.construct_lobby();
#ifndef INDEV
  main_app.net_manager->connect_to_master();
#endif
  main_app.w_lobby->show();

  return main_app.exec();
}
