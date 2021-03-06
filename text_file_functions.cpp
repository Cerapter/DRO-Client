#include "aoapplication.h"

#include "file_functions.h"

#include "aoconfig.h"

#include <QTextStream>
#include <QStringList>
#include <QVector>
#include <QDebug>
#include <QColor>

QString AOApplication::get_theme()
{
    return config->theme();
}

QString AOApplication::get_theme_variant()
{
    return config->theme_variant();
}

int AOApplication::read_blip_rate()
{
    return config->blip_rate();
}

bool AOApplication::read_chatlog_newline()
{
    return config->log_uses_newline_enabled();
}

int AOApplication::get_default_music()
{
    return config->music_volume();
}

int AOApplication::get_default_sfx()
{
    return config->effects_volume();
}

int AOApplication::get_default_blip()
{
    return config->blips_volume();
}

QStringList AOApplication::get_callwords()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    return config->callwords().split(" ", QString::SkipEmptyParts);
#else
    return config->callwords().split(" ", Qt::SkipEmptyParts);
#endif
}

QString AOApplication::read_note(QString filename)
{
    QFile note_txt(filename);

    if(!note_txt.open(QIODevice::ReadOnly | QFile::Text))
    {
        qDebug() << "Couldn't open" << filename;
        return "";
    }

    QTextStream in(&note_txt);
    QString text = in.readAll();
    note_txt.close();
    return text;
}

void AOApplication::write_note(QString p_text, QString p_file)
{
    QFile f_log(p_file);
    if(f_log.open(QIODevice::WriteOnly | QFile::Text))
    {
        QTextStream out(&f_log);

        out << p_text;

        f_log.flush();
        f_log.close();
    }
}

void AOApplication::append_note(QString p_line, QString p_file)
{
  QFile f_log(p_file);
  if(f_log.open(QIODevice::WriteOnly | QIODevice::Append))
    {
      QTextStream out(&f_log);

      out << p_line << "\r\n";

      f_log.flush();
      f_log.close();
    }
}

void AOApplication::write_to_serverlist_txt(QString p_line)
{
  QFile serverlist_txt;
  QString serverlist_txt_path = get_base_path() + "serverlist.txt";

  serverlist_txt.setFileName(serverlist_txt_path);

  if (!serverlist_txt.open(QIODevice::WriteOnly | QIODevice::Append))
  {
    return;
  }

  QTextStream out(&serverlist_txt);

  out << "\r\n" << p_line;

  serverlist_txt.close();
}

QVector<server_type> AOApplication::read_serverlist_txt()
{
  QVector<server_type> f_server_list;

  QFile serverlist_txt;
  QString serverlist_txt_path = get_base_path() + "serverlist.txt";

  serverlist_txt.setFileName(serverlist_txt_path);

  if (!serverlist_txt.open(QIODevice::ReadOnly))
  {
    return f_server_list;
  }

  QTextStream in(&serverlist_txt);

  while(!in.atEnd())
  {
    QString line = in.readLine();
    server_type f_server;
    QStringList line_contents = line.split(":");

    if (line_contents.size() < 3)
        continue;

    f_server.ip = line_contents.at(0);
    f_server.port = line_contents.at(1).toInt();
    f_server.name = line_contents.at(2);
    f_server.desc = "";

    f_server_list.append(f_server);
  }

  return f_server_list;
}

QString AOApplication::read_design_ini(QString p_identifier, QString p_design_path)
{
  QFile design_ini;

  design_ini.setFileName(p_design_path);

  if (!design_ini.open(QIODevice::ReadOnly))
  {
    return "";
  }
  QTextStream in(&design_ini);

  QString result = "";

  while (!in.atEnd())
  {
    QString f_line = in.readLine().trimmed();

    if (!f_line.startsWith(p_identifier))
      continue;

    QStringList line_elements = f_line.split("=");

    if (line_elements.at(0).trimmed() != p_identifier)
      continue;

    if (line_elements.size() < 2)
      continue;

    result = line_elements.at(1).trimmed();
    break;
  }

  design_ini.close();

  return result;
}

int AOApplication::get_design_ini_value(QString p_identifier, QString p_file)
{
  QString result = read_theme_ini(p_identifier, p_file);
  if (result.isEmpty())
    return 0;
  return result.toInt();
}

QPoint AOApplication::get_button_spacing(QString p_identifier, QString p_file)
{
  QPoint return_value;
  return_value.setX(0);
  return_value.setY(0);

  QString f_result = read_theme_ini(p_identifier, p_file);
  if (f_result.isEmpty())
    return return_value;

  QStringList sub_line_elements = f_result.split(",");
  if (sub_line_elements.size() < 2)
    return return_value;

  return_value.setX(sub_line_elements.at(0).toInt());
  return_value.setY(sub_line_elements.at(1).toInt());

  return return_value;
}

pos_size_type AOApplication::get_element_dimensions(QString p_identifier, QString p_file)
{
  pos_size_type return_value;
  return_value.x = 0;
  return_value.y = 0;
  return_value.width = -1;
  return_value.height = -1;

  QString f_result = read_theme_ini(p_identifier, p_file);
  if (f_result.isEmpty())
    return return_value;

  QStringList sub_line_elements = f_result.split(",");

  if (sub_line_elements.size() < 4)
    return return_value;

  return_value.x = sub_line_elements.at(0).toInt();
  return_value.y = sub_line_elements.at(1).toInt();
  return_value.width = sub_line_elements.at(2).toInt();
  return_value.height = sub_line_elements.at(3).toInt();

  return return_value;
}

int AOApplication::get_font_property(QString p_identifier, QString p_file)
{
  QString f_result = read_theme_ini(p_identifier, p_file);

  if (f_result.isEmpty())
    return 0;
  return f_result.toInt();
}

QColor AOApplication::get_color(QString p_identifier, QString p_file)
{
  QColor return_color(255, 255, 255);

  QString f_result = read_theme_ini(p_identifier, p_file);
  if (f_result.isEmpty())
    return return_color;

  QStringList color_list = f_result.split(",");

  if (color_list.size() < 3)
    return return_color;

  return_color.setRed(color_list.at(0).toInt());
  return_color.setGreen(color_list.at(1).toInt());
  return_color.setBlue(color_list.at(2).toInt());

  return return_color;
}

QString AOApplication::get_font_name(QString p_identifier, QString p_file)
{
  QString f_result = read_theme_ini(p_identifier, p_file);

  if (f_result.isEmpty())
    qDebug() << "Failure retreiving font name";

  return f_result;
}

QString AOApplication::get_sfx(QString p_identifier)
{
  return read_theme_ini(p_identifier, "courtroom_sounds.ini");
}

QString AOApplication::get_stylesheet(QString target_tag, QString p_file)
{
  QStringList paths{get_theme_variant_path() + p_file, get_theme_path() + p_file};

  for (QString path : paths)
  {
    QFile design_ini;
    design_ini.setFileName(path);
    if (!design_ini.open(QIODevice::ReadOnly))
      continue;

    QTextStream in(&design_ini);
    QString f_text;
    bool tag_found = false;

    while (!in.atEnd())
    {
      QString line = in.readLine();
      if (line.startsWith(target_tag, Qt::CaseInsensitive))
      {
        tag_found = true;
        continue;
      }

      if (tag_found)
      {
        if ((line.startsWith("[") && line.endsWith("]")))
          break;
        f_text.append(line);
      }
    }

    design_ini.close();
    if (!f_text.isEmpty())
      return f_text;
  }

  // Default value in case everything fails, return an empty string
  return "";
}

QVector<QStringList> AOApplication::get_highlight_color()
{
  QString p_file = "courtroom_config.ini";
  QStringList paths{get_theme_variant_path() + p_file, get_theme_path() + p_file};

  for (QString path : paths)
  {
    QVector<QStringList> f_vec;

    QFile design_ini;
    design_ini.setFileName(path);
    if (!design_ini.open(QIODevice::ReadOnly))
      continue;

    QTextStream in(&design_ini);
    bool tag_found = false;

    while (!in.atEnd())
    {
      QString line = in.readLine();

      if (line.startsWith("[HIGHLIGHTS]", Qt::CaseInsensitive))
      {
        tag_found = true;
        continue;
      }

      if (tag_found)
      {
        if((line.startsWith("[") && line.endsWith("]")))
          break;
        // Syntax
        // OpenercharCloserchar = Color, Shown
        // Shown is 1 if the character should be displayed in IC, 0 otherwise. If not present,
        // assume 1.
        QString chars = line.split("=")[0].trimmed();
        QString chars_parameters = line.mid(line.indexOf("=")+1);
        QStringList parameters = chars_parameters.split(",");
        for (int i=0; i<parameters.size(); i++)
          parameters[i] = parameters[i].trimmed();
        if (parameters.size() == 1)
          parameters.append("1");
        f_vec.append({chars, parameters[0], parameters[1]});
      }
    }

    design_ini.close();
    if (!f_vec.isEmpty())
      return f_vec;
  }

  // Default value in case everything fails, return an empty vector
  QVector<QStringList> f_vec;
  return f_vec;
}

QString AOApplication::get_spbutton(QString p_tag, int index)
{
  QString p_file = "courtroom_config.ini";
  QStringList paths{get_theme_variant_path() + p_file, get_theme_path() + p_file};

  for (QString path : paths)
  {
    QString res = "";

    QFile design_ini;
    design_ini.setFileName(path);
    if (!design_ini.open(QIODevice::ReadOnly))
      continue;

    QTextStream in(&design_ini);
    bool tag_found = false;

    while (!in.atEnd())
    {
      QString line = in.readLine();

      if (line.startsWith(p_tag, Qt::CaseInsensitive))
      {
        tag_found = true;
        continue;
      }

      if (tag_found)
      {
        if ((line.startsWith("[") && line.endsWith("]")))
           break;

        QStringList line_contents = line.split("=");
        if (line_contents.at(0).trimmed() == QString::number(index))
          res = line_contents.at(1);
      }
    }

    design_ini.close();
    if (!res.isEmpty())
      return res;
  }

  // Default value in case everything fails, return an empty string
  return "";
}

QStringList AOApplication::get_effect(int index)
{
  QString p_file = "courtroom_config.ini";
  QStringList paths{get_theme_variant_path() + p_file, get_theme_path() + p_file};

  for (QString path : paths)
  {
    QStringList res;

    QFile design_ini;
    design_ini.setFileName(path);
    if (!design_ini.open(QIODevice::ReadOnly))
      continue;

    QTextStream in(&design_ini);
    bool tag_found = false;

    while (!in.atEnd())
    {
      QString line = in.readLine();

      if (line.startsWith("[EFFECTS]", Qt::CaseInsensitive))
      {
        tag_found = true;
        continue;
      }

      if (tag_found)
      {
        if ((line.startsWith("[") && line.endsWith("]")))
          break;

        QStringList line_contents = line.split("=");
        if (line_contents.at(0).trimmed() == QString::number(index))
          res = line_contents.at(1).split(",");

        if (res.size() == 1)
          res.append("1");
      }
    }

    design_ini.close();
    if (!res.isEmpty())
      return res;
  }

  // Default value in case everything fails, return an empty string list
  QStringList res;
  return res;
}

QStringList AOApplication::get_sfx_list()
{
  QStringList return_value;
  QFile base_sfx_list_ini;
  QFile char_sfx_list_ini;

  base_sfx_list_ini.setFileName(get_base_path() + "configs/sounds.ini");
  char_sfx_list_ini.setFileName(get_character_path(get_current_char()) + "sounds.ini");

  if (!char_sfx_list_ini.open(QIODevice::ReadOnly) && !base_sfx_list_ini.open(QIODevice::ReadOnly))
  {
    return return_value;
  }

  QTextStream in_a(&base_sfx_list_ini);
  QTextStream in_b(&char_sfx_list_ini);

  while (!in_a.atEnd())
  {
    QString line = in_a.readLine();
    return_value.append(line);
  }

  while (!in_b.atEnd())
  {
    QString line = in_b.readLine();
    return_value.append(line);
  }

  return return_value;
}

//returns whatever is to the right of "search_line =" within target_tag and terminator_tag, trimmed
//returns the empty string if the search line couldnt be found
QString AOApplication::read_char_ini(QString p_char, QString p_search_line, QString target_tag, QString terminator_tag)
{
  QString char_ini_path = get_character_path(p_char) + "char.ini";

  QFile char_ini;

  char_ini.setFileName(char_ini_path);

  if (!char_ini.open(QIODevice::ReadOnly))
    return "";

  QTextStream in(&char_ini);

  bool tag_found = false;

  while(!in.atEnd())
  {
    QString line = in.readLine();

    if (QString::compare(line, terminator_tag, Qt::CaseInsensitive) == 0)
      break;

    if (line.startsWith(target_tag, Qt::CaseInsensitive))
    {
      tag_found = true;
      continue;
    }

    if (!line.startsWith(p_search_line, Qt::CaseInsensitive))
      continue;

    QStringList line_elements = line.split("=");

    if (QString::compare(line_elements.at(0).trimmed(), p_search_line, Qt::CaseInsensitive) != 0)
      continue;

    if (line_elements.size() < 2)
      continue;

    if (tag_found)
    {
      char_ini.close();
      return line_elements.at(1).trimmed();
    }
  }

  char_ini.close();
  return "";
}

QString AOApplication::get_char_name(QString p_char)
{
  QString f_result = read_char_ini(p_char, "name", "[Options]", "[Time]");

  if (f_result == "")
    return p_char;
  else return f_result;
}

QString AOApplication::get_showname(QString p_char)
{
  QString f_result = read_showname(p_char);
  if(f_result == "")
    f_result = read_char_ini(p_char, "showname", "[Options]", "[Time]");

  if (f_result == "")
    return p_char;
  else return f_result;
}

QString AOApplication::read_showname(QString p_char)
{
  QString f_filename = get_base_path() + "configs/shownames.ini";
  QFile f_file(f_filename);
  if(!f_file.open(QIODevice::ReadOnly))
  { qDebug() << "Error reading" << f_filename; return ""; }

  QTextStream in(&f_file);
  while(!in.atEnd())
  {
    QString f_line = in.readLine();
    if(!f_line.startsWith(p_char))
      continue;

    QStringList line_elements = f_line.split("=");
    if(line_elements.at(0).trimmed() == p_char)
      return line_elements.at(1).trimmed();
  }
  return "";
}

QString AOApplication::get_char_side(QString p_char)
{
  QString f_result = read_char_ini(p_char, "side", "[Options]", "[Time]");

  if (f_result == "")
    return "wit";
  else return f_result;
}

QString AOApplication::get_gender(QString p_char)
{
  QString f_result = read_char_ini(p_char, "gender", "[Options]", "[Time]");

  if (f_result == "")
    return "male";
  else return f_result;
}

QString AOApplication::get_chat(QString p_char)
{
  QString f_result = read_char_ini(p_char, "chat", "[Options]", "[Time]");

  //handling the correct order of chat is a bit complicated, we let the caller do it
  return f_result.toLower();
}

QString AOApplication::get_char_shouts(QString p_char)
{
  QString f_result = read_char_ini(p_char, "shouts", "[Options]", "[Time]");

  return f_result.toLower();
}

int AOApplication::get_emote_number(QString p_char)
{
  QString f_result = read_char_ini(p_char, "number", "[Emotions]", "[Offsets]");

  if (f_result == "")
    return 0;
  else return f_result.toInt();
}

QString AOApplication::get_emote_comment(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "[Emotions]", "[Offsets]");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 4)
  {
    qDebug() << "W: misformatted char.ini: " << p_char << ", " << p_emote;
    return "normal";
  }
  else return result_contents.at(0);
}

QString AOApplication::get_pre_emote(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "[Emotions]", "[Offsets]");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 4)
  {
    qDebug() << "W: misformatted char.ini: " << p_char << ", " << p_emote;
    return "";
  }
  else return result_contents.at(1);
}

QString AOApplication::get_emote(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "[Emotions]", "[Offsets]");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 4)
  {
    qDebug() << "W: misformatted char.ini: " << p_char << ", " << p_emote;
    return "normal";
  }
  else return result_contents.at(2);
}

int AOApplication::get_emote_mod(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "[Emotions]", "[Offsets]");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 4)
  {
    qDebug() << "W: misformatted char.ini: " << p_char << ", " << QString::number(p_emote);
    return 0;
  }
  else return result_contents.at(3).toInt();
}

int AOApplication::get_desk_mod(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "[Emotions]", "[Offsets]");

  QStringList result_contents = f_result.split("#");

  if (result_contents.size() < 5)
    return -1;

  QString string_result = result_contents.at(4);
  if (string_result == "")
    return -1;

  else return string_result.toInt();
}

QStringList AOApplication::get_effect_offset(QString p_char, int p_effect)
{
  QStringList f_result = read_char_ini(p_char, QString::number(p_effect), "[Offsets]", "[Overlay]").split(",");

  if (f_result.size() < 2)
    return decltype(f_result) {0, 0};

  return f_result;
}

QStringList AOApplication::get_overlay(QString p_char, int p_effect)
{
  QStringList f_result = read_char_ini(p_char, QString::number(p_effect), "[Overlay]", "[SoundN]").split("#");

  if (f_result.size() < 2)
    f_result.push_back("");

  return f_result;
}

QString AOApplication::get_sfx_name(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "[SoundN]", "[SoundT]");

  if (f_result == "")
    return "1";
  else return f_result;
}

int AOApplication::get_sfx_delay(QString p_char, int p_emote)
{
  QString f_result = read_char_ini(p_char, QString::number(p_emote + 1), "[SoundT]", "[TextDelay]");

  if (f_result == "")
    return 1;
  else return f_result.toInt();
}

int AOApplication::get_text_delay(QString p_char, QString p_emote)
{
  QString f_result = read_char_ini(p_char, p_emote, "[TextDelay]", "END_OF_FILE");

  if (f_result == "")
    return -1;
  else return f_result.toInt();
}

bool AOApplication::get_blank_blip()
{
    return config->blank_blips_enabled();
}

QString AOApplication::read_theme_ini(QString p_identifier, QString p_file)
{
  // Try to obtain a theme ini from either the current theme variant folder,
  // the current theme folder or the default theme folder
  QStringList paths{
    get_theme_variant_path() + p_file,
    get_theme_path() + p_file,
    get_default_theme_path() + p_file,
  };

  for (QString path: paths)
  {
    QString f_result = read_design_ini(p_identifier, path);
    if (!f_result.isEmpty())
      return f_result;
  }

  return "";
}

QString AOApplication::get_image_path(QString p_image)
{
  QString theme_variant_image_path = get_theme_variant_path() + p_image;
  QString theme_image_path = get_theme_path() + p_image;
  QString default_image_path = get_default_theme_path() + p_image;

  QString final_image_path;

  if (file_exists(theme_variant_image_path))
    return theme_variant_image_path;
  else if (file_exists(theme_image_path))
    return theme_image_path;
  return default_image_path;
}
