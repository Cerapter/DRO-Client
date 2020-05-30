#ifndef AOSCENE_H
#define AOSCENE_H

#include <QLabel>

class Courtroom;
class AOApplication;

class AOScene : public QLabel
{
  Q_OBJECT
public:
  explicit AOScene(QWidget *parent, AOApplication *p_ao_app);

  void set_image(QString p_image);

private:
  QWidget*       m_parent = nullptr;
  QMovie*        m_movie = nullptr;
  AOApplication* ao_app = nullptr;

};

#endif // AOSCENE_H
