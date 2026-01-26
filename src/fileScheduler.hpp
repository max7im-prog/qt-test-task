#pragma once
#include <QMutex>
#include <QObject>
#include <QTimer>
#include <qobject.h>
#include <qtmetamacros.h>

class FileScheduler : public QObject {
  Q_OBJECT

public:
  struct Task {
    bool _run;
    QString _inputFileMask;
    QString _fromDirectory;
    QString _toDirectory;
    enum class FileRepeatAction { Rewrite, Pass, Copy } _fileRepeatAction;
    int _intervalMS;
    bool _repeatRun;
  };
  explicit FileScheduler(const Task &task, QObject *parent = nullptr);
  Task getTask() const;

private:
  Task _task;
  QMutex _taskAccessMutex;
  QTimer _timer;
  void applyTask();

public slots:
  void onTimer();
  void onSetTask(const Task &task);

signals:
  void showUserInfo(const QString &info);
};
