
#include "oglwindow.h"
#include "workerthread.h"
extern WorkerThread* workerThread;

void OGLWindow::initializeGL() {
    doneCurrent();
    context()->moveToThread(workerThread);
}

void OGLWindow::keyPressEvent(QKeyEvent *event)
{
    int modValue = QT2SDL2MOD(event->modifiers());
    int keyValue = QT2SDL2(event->key());
    if (keyValue != 0)
        (*CoreDoCommand)(M64CMD_SEND_SDL_KEYDOWN, (modValue << 16) + keyValue, NULL);
}

void OGLWindow::keyReleaseEvent(QKeyEvent *event)
{
    int modValue = QT2SDL2MOD(event->modifiers());
    int keyValue = QT2SDL2(event->key());
    if (keyValue != 0)
        (*CoreDoCommand)(M64CMD_SEND_SDL_KEYUP, (modValue << 16) + keyValue, NULL);
}

void OGLWindow::resizeEvent(QResizeEvent *event) {
    QOpenGLWindow::resizeEvent(event);
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    }
    timerId = startTimer(500);
    m_width = event->size().width();
    m_height = event->size().height();
}

void OGLWindow::timerEvent(QTimerEvent *te) {
    int size = (m_width << 16) + m_height;
    (*CoreDoCommand)(M64CMD_CORE_STATE_SET, M64CORE_VIDEO_SIZE, &size);
    killTimer(te->timerId());
    timerId = 0;
}
