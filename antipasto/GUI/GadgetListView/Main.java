package antipasto.GUI.GadgetListView;

import javax.swing.*;

import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveSketchChangingListener;

import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;



public class Main {
    public static void main(String[] args) {
        JFrame frame = new JFrame();
        frame.setLayout(new FlowLayout());
        final GadgetPanel wind = new GadgetPanel("C:\\OpenHardware\\Gadgets\\Slide.pde", frame);
        IActiveSketchChangingListener listenr;
        frame.setSize(250, 600);
        frame.add(wind);
        frame.setVisible(true);
         frame.addWindowListener(new WindowAdapter() {
        public void windowClosing(WindowEvent e) {
          wind.saveCurrentGadget();
        }
      });
    }
}
