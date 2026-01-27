#include "appController.hpp"

void AppController::start() {
  // TODO: complete
}

void AppController::stop() {

  // TODO: complete
}

const QString &AppController::getStatus() const { return _status; }

const QString &AppController::getInputDir() const { return _inputDir; }

const QString &AppController::getOutputDir() const { return _outputDir; }

const QString &AppController::getFileMask() const { return _fileMask; }

const QString &AppController::getByteMask() const { return _byteMask; }

int AppController::getQueryIntervalMs() const { return _queryIntervalMs; }

bool AppController::getSingleShot() const { return _singleShot; }

void AppController::setStatus(const QString &param) { _status = param; }

void AppController::setInputDir(const QString &param) { _inputDir = param; }

void AppController::setOutputDir(const QString &param) { _outputDir = param; }

void AppController::setFileMask(const QString &param) { _fileMask = param; }

void AppController::setByteMask(const QString &param) { _byteMask = param; }

void AppController::setQueryIntervalMs(int param) { _queryIntervalMs = param; }

void AppController::setSingleShot(bool param) { _singleShot = param; }
