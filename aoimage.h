//This class represents a static theme-dependent image

#ifndef AOIMAGE_H
#define AOIMAGE_H

#include "aoapplication.h"

#include <QLabel>

class AOImage : public QLabel
{
public:
  AOImage(QWidget *parent, AOApplication *p_ao_app);
  ~AOImage();

  AOApplication *ao_app = nullptr;

  void set_image(QString p_image);
  void set_image_from_path(QString p_path);
  void set_cropped_image(QString p_image, int x, int y, int width, int height);
  void set_size_and_pos(QString identifier);
};

#endif // AOIMAGE_H
