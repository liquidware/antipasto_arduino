package processing.app;

import java.awt.*;
import javax.swing.*;

public class ExceptionHandler implements Thread.UncaughtExceptionHandler {

    public void uncaughtException(final Thread t, final Throwable e) {
        if (SwingUtilities.isEventDispatchThread()) {
            showException(t, e);
        } else {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    showException(t, e);
                    System.exit(-1);
                }
            });
        }
    }

    private void showException(Thread t, Throwable e) {
        String msg = String.format("Unexpected problem on thread %s: %s",
                t.getName(), e.getMessage());

        logException(t, e);

        JOptionPane pane = new JOptionPane(msg, 
                                           JOptionPane.QUESTION_MESSAGE);

        JDialog dialog = pane.createDialog(Base.editor, 
                                           "Unexpected Problem");
        dialog.setModalityType(Dialog.ModalityType.APPLICATION_MODAL);
        dialog.setVisible(true);
    }

    private void logException(Thread t, Throwable e) {
        // todo: start a thread that sends an email, or write to a log file, or
        // send a JMS message...whatever
    }
}
