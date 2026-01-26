#pragma once
#include "fileModifier.hpp"
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
    std::chrono::milliseconds _queryInterval;
    bool _singleShot;
    int _maxconcurrentProcesses;
  };
  explicit FileScheduler(const Task &task, QObject *parent = nullptr);
  Task getTask() const;

private:
  Task _task;
  QMutex _taskAccessMutex;
  QTimer _queryTimer;
  int _runningThreads;
  void applyTask();
  void processQuery();

public slots:
  void onTimer();
  void onSetTask(const Task &task);
  void onModifierProgress(const FileModifier::Progress &progress);
  void onModifierFinished(const FileModifier::Progress &progress);

signals:
  void showUserInfo(const QString &info);
};
