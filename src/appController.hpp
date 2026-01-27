#pragma once
#include "fileScheduler.hpp"
#include <qobject.h>
#include <qtmetamacros.h>

class AppController : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString status READ getStatus WRITE setStatus NOTIFY statusChanged)
  Q_PROPERTY(QString inputDir READ getInputDir WRITE setInputDir NOTIFY
                 inputDirChanged)
  Q_PROPERTY(QString outputDir READ getOutputDir WRITE setOutputDir NOTIFY
                 outputDirChanged)
  Q_PROPERTY(QString fileMask READ getFileMask WRITE setFileMask NOTIFY
                 fileMaskChanged)
  Q_PROPERTY(QString byteMask READ getByteMask WRITE setByteMask NOTIFY
                 byteMaskChanged)
  Q_PROPERTY(int queryIntervalMs READ getQueryIntervalMs WRITE
                 setQueryIntervalMs NOTIFY queryIntervalMsChanged)
  Q_PROPERTY(bool singleShot READ getSingleShot WRITE setSingleShot NOTIFY
                 singleShotChanged)

public:
  explicit AppController(QObject *parent = nullptr);

  Q_INVOKABLE void start();
  Q_INVOKABLE void stop();

  const QString &getStatus() const;
  const QString &getInputDir() const;
  const QString &getOutputDir() const;
  const QString &getFileMask() const;
  const QString &getByteMask() const;
  int getQueryIntervalMs() const;
  bool getSingleShot() const;

  void setStatus(const QString &param);
  void setInputDir(const QString &param);
  void setOutputDir(const QString &param);
  void setFileMask(const QString &param);
  void setByteMask(const QString &param);
  void setQueryIntervalMs(int param);
  void setSingleShot(bool param);

signals:
  void statusChanged();
  void inputDirChanged();
  void outputDirChanged();
  void fileMasChanged();
  void byteMaskChanged();
  void queryIntervalMsChanged();
  void singleShotChanged();

private:
  FileScheduler _scheduler;
  QString _status;
  QString _inputDir;
  QString _outputDir;
  QString _fileMask;
  QString _byteMask;
  int _queryIntervalMs;
  bool _singleShot;
};
