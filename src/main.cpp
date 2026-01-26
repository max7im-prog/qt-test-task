#include <QObject>
#include <iostream>

int main() {
  std::atomic<bool> running = true;

  while (running) {
  }

  return 0;
}
