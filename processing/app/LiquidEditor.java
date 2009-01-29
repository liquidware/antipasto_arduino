package processing.app;

import javax.swing.*;
import java.awt.*;

public class LiquidEditor extends JFrame {
    public void LiquidEditor(Editor editor){
        this.setLayout(new BorderLayout());
        this.add(BorderLayout.CENTER, editor);
    }
}
