#pragma once
#include <QObject>
#include <qstringview.h>
#include <qtmetamacros.h>

class FileModifier : public QObject {
  Q_OBJECT

public:
  struct Task {
    QString _fromPath;
    QString _toPath;
    QByteArray _byteMask;
  };
  explicit FileModifier(const Task &task, QObject *parent = nullptr);

private:
  const Task _task;
public slots:
  void onProcess();
signals:
  void finished();
  void progress();
};
