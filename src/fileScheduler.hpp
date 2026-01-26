#pragma once
#include "fileModifier.hpp"
#include <QMutex>
#include <QObject>
#include <QSet>
#include <QTimer>
#include <qobject.h>
#include <qstringview.h>
#include <qtmetamacros.h>

class FileScheduler : public QObject {
  Q_OBJECT

public:
  struct Task {
    bool _run;
    QByteArray _byteMask;
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
  void applyTask();
  void processQuery();
  void scheduleModifier(const FileModifier::Task &task);

  Task _task;
  QMutex _taskAccessMutex;
  QTimer _queryTimer;
  int _runningThreads;
  QSet<QString> _activeFiles;

public slots:
  void onTimer();
  void onSetTask(const Task &task);
  void onModifierProgress(const FileModifier::Progress &progress);
  void onModifierFinished(const FileModifier::Progress &progress);

signals:
  void showUserInfo(const QString &info);
};
