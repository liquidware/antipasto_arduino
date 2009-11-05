/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Processing project - http://processing.org

  Copyright (c) 2004-05 Ben Fry and Casey Reas
  Copyright (c) 2001-04 Massachusetts Institute of Technology

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  $Id: EditorStatus.java 346 2007-10-06 20:26:45Z mellis $
*/

package processing.app;

import java.io.*;
import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.JCheckBox;

import antipasto.ModuleRules.IOkListener;
import antipasto.ModuleRules.ISerialListener;
import antipasto.ModuleRules.IYesNoListener;

//import processing.app.InfiniteProgressPanel;

/**
 * Panel just below the editing area that contains status messages.
 */
public class EditorStatus extends JPanel implements ActionListener {
    static Color bgcolor[];
    static Color fgcolor[];

    static final int NOTICE = 0;
    static final int ERR    = 1;
    static final int PROMPT = 2;
    static final int EDIT   = 3;
    static final int SERIAL = 4;

    static final int YES    = 1;
    static final int NO     = 2;
    static final int CANCEL = 3;
    static final int OK     = 4;
    static final int SEND   = 5;

    static final String NO_MESSAGE = "";

    Editor editor;

    int mode;
    String message;

    Font font;
    FontMetrics metrics;
    int ascent;

    Image offscreen;
    int sizeW, sizeH;
    int imageW, imageH;

    JButton yesButton;
    JButton noButton;
    JButton cancelButton;
    JButton okButton;
    JButton sendButton;
    JTextField editField;
    JTextField serialField;
    JComboBox serialRates;
    JCheckBox newlineChkBox;

    //Thread promptThread;
    int response;
    InfiniteProgressPanel activityIndicator;
    boolean activityIndicatorRunning = false;

    public EditorStatus(Editor editor) {
        this.editor = editor;
        empty();

        if (bgcolor == null) {
            bgcolor = new Color[5];
            // Arduino 0003 switched to a blue color scheme to visually distinguish
            // itself from Processing.  Because the image files for certain interface
            // elements (e.g. buttons and tabs) are distributed with the application
            // while the preference file that specifies the IDE colors is stored in
            // the user's home directory and shared across all versions of Arduino,
            // we need to hardcode certain colors here to match the images.
            // Otherwise, users who used different multiple versions of the software
            // would sometimes see colors that didn't match the interface elements.
            // This is a hack and prevents users from customizing the IDE colors,
            // however, it obviates the need to provide for version-specific
            // preferences.
            //bgcolor[0] = Preferences.getColor("status.notice.bgcolor");
            bgcolor[0] = new Color(0x54, 0x91, 0x9e);
            bgcolor[1] = Preferences.getColor("status.error.bgcolor");
            bgcolor[2] = Preferences.getColor("status.prompt.bgcolor");
            bgcolor[3] = Preferences.getColor("status.prompt.bgcolor");
            bgcolor[4] = new Color(0x54, 0x91, 0x9e);

            fgcolor = new Color[5];
            fgcolor[0] = Preferences.getColor("status.notice.fgcolor");
            fgcolor[1] = Preferences.getColor("status.error.fgcolor");
            fgcolor[2] = Preferences.getColor("status.prompt.fgcolor");
            fgcolor[3] = Preferences.getColor("status.prompt.fgcolor");
            fgcolor[4] = Preferences.getColor("status.notice.fgcolor");
        }
    }


    public void empty() {
        mode = NOTICE;
        message = NO_MESSAGE;
        //update();
        repaint();
    }

    public void notice(String message, boolean showActivity) {
        if (showActivity) {
            activityIndicator.start();
            activityIndicatorRunning = true;
        } else {
            activityIndicator.stop();
            activityIndicatorRunning = false;
        }
        notice(message);
    }

    public void notice(String message) {
        mode = NOTICE;
        this.message = message;
        //update();
        repaint();
    }

    public void unnotice(String unmessage) {
        if (message.equals(unmessage)) empty();
    }


    public void error(String message) {
        activityIndicator.stop();
        activityIndicatorRunning = false;
        mode = ERR;
        this.message = message;
        repaint();

    }


    public void prompt(String message) {
        mode = PROMPT;
        this.message = message;

        response = 0;
        yesButton.setVisible(true);
        noButton.setVisible(true);
        cancelButton.setVisible(true);
        yesButton.requestFocus();

        repaint();
    }


    // prompt has been handled, re-hide the buttons
    public void unprompt() {
        yesButton.setVisible(false);
        noButton.setVisible(false);
        cancelButton.setVisible(false);
        empty();
    }


    public void edit(String message, String dflt) {
        mode = EDIT;
        this.message = message;

        response = 0;
        okButton.setVisible(true);
        cancelButton.setVisible(true);
        editField.setVisible(true);
        editField.setText(dflt);
        editField.selectAll();
        editField.requestFocus();

        repaint();
    }

    public void unedit() {
        okButton.setVisible(false);
        cancelButton.setVisible(false);
        editField.setVisible(false);
        empty();
    }

    public void serial() {
        mode = SERIAL;
        this.message = NO_MESSAGE;

        sendButton.setVisible(true);
        serialRates.setVisible(true);
        serialField.setVisible(true);
        newlineChkBox.setVisible(true);
        serialField.setText("");
        serialField.requestFocus();

        repaint();
    }

    public void unserial() {
        sendButton.setVisible(false);
        newlineChkBox.setVisible(false);
        serialField.setVisible(false);
        serialRates.setVisible(false);
        empty();
    }

    public void paintComponent(Graphics screen) {

        int textHOffset = 0;

        if (activityIndicatorRunning) {
            textHOffset = 32;             //to make room for the activity indicator
        }

        if (yesButton == null) setup();

        Dimension size = getSize();
        setButtonBounds();
        if ((size.width != sizeW) || (size.height != sizeH)) {
            // component has been resized

            if ((size.width > imageW) || (size.height > imageH)) {
                // nix the image and recreate, it's too small
                offscreen = null;

            } else {
                // who cares, just resize
                sizeW = size.width;
                sizeH = size.height;
                setButtonBounds();
            }
        }

        if (offscreen == null) {
            sizeW = size.width;
            sizeH = size.height;
            setButtonBounds();
            imageW = sizeW;
            imageH = sizeH;
            offscreen = createImage(imageW, imageH);
        }

        Graphics g = offscreen.getGraphics();
        if (font == null) {
            font = Preferences.getFont("status.font");
            g.setFont(font);
            metrics = g.getFontMetrics();
            ascent = metrics.getAscent();
        }

        g.setColor(bgcolor[mode]);
        g.fillRect(0, 0, imageW, imageH);

        g.setColor(fgcolor[mode]);
        g.setFont(font); // needs to be set each time on osx
        g.drawString(message, Preferences.GUI_SMALL + textHOffset, (sizeH + ascent) / 2);

        screen.drawImage(offscreen, 0, 0, null);
    }


    protected void setup() {
        if (yesButton == null) {
            yesButton    = new JButton(Preferences.PROMPT_YES);
            noButton     = new JButton(Preferences.PROMPT_NO);
            cancelButton = new JButton(Preferences.PROMPT_CANCEL);
            okButton     = new JButton(Preferences.PROMPT_OK);
            sendButton   = new JButton(Preferences.PROMPT_SEND);

            // !@#(* aqua ui #($*(( that turtle-neck wearing #(** (#$@)(
            // os9 seems to work if bg of component is set, but x still a bastard
            if (Base.isMacOS()) {
                yesButton.setBackground(bgcolor[PROMPT]);
                noButton.setBackground(bgcolor[PROMPT]);
                cancelButton.setBackground(bgcolor[PROMPT]);
                okButton.setBackground(bgcolor[PROMPT]);
                sendButton.setBackground(bgcolor[SERIAL]);
            }
            setLayout(null);

            yesButton.addActionListener(this);
            noButton.addActionListener(this);
            cancelButton.addActionListener(this);
            okButton.addActionListener(this);
            sendButton.addActionListener(this);

            add(yesButton);
            add(noButton);
            add(cancelButton);
            add(okButton);

            activityIndicator = new InfiniteProgressPanel();
            activityIndicator.setPreferredSize(new Dimension(24,24));
            add(activityIndicator);

            activityIndicator.stop();

            yesButton.setVisible(false);
            noButton.setVisible(false);
            cancelButton.setVisible(false);
            okButton.setVisible(false);
            sendButton.setVisible(false);

            editField = new JTextField();
            editField.addActionListener(this);

            editField.addKeyListener(new KeyAdapter() {


                                         // use keyTyped to catch when the feller is actually
                                         // added to the text field. with keyTyped, as opposed to
                                         // keyPressed, the keyCode will be zero, even if it's
                                         // enter or backspace or whatever, so the keychar should
                                         // be used instead. grr.
                                         public void keyTyped(KeyEvent event) {
                                             int c = event.getKeyChar();

                                             if (mode == EDIT) {
                                                 if (c == KeyEvent.VK_ENTER) {  // accept the input
                                                     String answer = editField.getText();
                                                     editor.sketch.nameCode(answer);
                                                     unedit();
                                                     event.consume();

                                                     // easier to test the affirmative case than the negative
                                                 } else if ((c == KeyEvent.VK_BACK_SPACE) ||
                                                            (c == KeyEvent.VK_DELETE) ||
                                                            (c == KeyEvent.VK_RIGHT) ||
                                                            (c == KeyEvent.VK_LEFT) ||
                                                            (c == KeyEvent.VK_UP) ||
                                                            (c == KeyEvent.VK_DOWN) ||
                                                            (c == KeyEvent.VK_HOME) ||
                                                            (c == KeyEvent.VK_END) ||
                                                            (c == KeyEvent.VK_SHIFT)) {

                                                 } else if (c == KeyEvent.VK_ESCAPE) {
                                                     unedit();
                                                     editor.buttons.clear();
                                                     event.consume();

                                                 } else if (c == KeyEvent.VK_SPACE) {
                                                     String t = editField.getText();
                                                     int start = editField.getSelectionStart();
                                                     int end = editField.getSelectionEnd();
                                                     editField.setText(t.substring(0, start) + "_" +
                                                                       t.substring(end));
                                                     editField.setCaretPosition(start+1);
                                                     event.consume();

                                                 } else if ((c == '_') || (c == '.') ||  // allow .pde and .java
                                                            ((c >= 'A') && (c <= 'Z')) ||
                                                            ((c >= 'a') && (c <= 'z'))) {
                                                     // everything fine, catches upper and lower
                                                 } else if ((c >= '0') && (c <= '9')) {
                                                     // getCaretPosition == 0 means that it's the first char
                                                     // and the field is empty.
                                                     // getSelectionStart means that it *will be* the first
                                                     // char, because the selection is about to be replaced
                                                     // with whatever is typed.
                                                     if ((editField.getCaretPosition() == 0) ||
                                                         (editField.getSelectionStart() == 0)) {
                                                         // number not allowed as first digit
                                                         event.consume();
                                                     }
                                                 } else {
                                                     event.consume();
                                                 }
                                             }
                                         }
                                     });
            add(editField);
            editField.setVisible(false);

            this.setLayout(new FlowLayout(FlowLayout.LEFT,10,5));

            initSerialRates();
            initNewLineCheckBox();
            initSerialField();

            add(serialRates);
            add(newlineChkBox);
            add(serialField);
            add(sendButton);

        }
    }

    public void initSerialRates() {

        String[] serialRateStrings = {
            "300","1200","2400","4800","9600","14400",
            "19200","28800","38400","57600","115200"
        };

        //Serial Rates
        serialRates = new JComboBox();

        if (Base.isMacOS())
            serialRates.setBackground(bgcolor[SERIAL]);

        for (int i = 0; i < serialRateStrings.length; i++)
            serialRates.addItem(serialRateStrings[i] + " baud");

        serialRates.setSelectedItem(Preferences.get("serial.debug_rate") + " baud");
        serialRates.addActionListener(this);
        serialRates.setVisible(false);

    }

    public void initNewLineCheckBox() {

        newlineChkBox = new JCheckBox("Send New Line",
                                      Preferences.getBoolean("serial.sendNewLine"));
        newlineChkBox.setBackground(bgcolor[SERIAL]);
        newlineChkBox.setForeground(new Color(0xFF,0xFF,0xFF));

        ActionListener newlineChkBoxListener = new ActionListener() {
            public void actionPerformed(ActionEvent ev) {
                JCheckBox checkbox = (JCheckBox)ev.getSource();
                if (checkbox.isSelected()) {
                    Preferences.set("serial.sendNewLine","true");
                } else {
                    Preferences.set("serial.sendNewLine","false");
                }
            }
        };

        newlineChkBox.addActionListener(newlineChkBoxListener);
        newlineChkBox.setVisible(false);
    }

    public void initSerialField() {

        //Serial Field
        serialField = new JTextField();
        serialField.addActionListener(this);

        serialField.addKeyListener(new KeyAdapter() {
                                       public void keyTyped(KeyEvent event) {
                                           int c = event.getKeyChar();

                                           if (c == KeyEvent.VK_ENTER) {  // accept the input
                                               editor.serialPort.write(serialField.getText());
                                               if (Preferences.getBoolean("serial.sendNewLine")) {
                                                   editor.serialPort.write('\n');
                                               }
                                               event.consume();
                                               serialField.setText("");
                                           }
                                       }
                                   });

        serialField.setPreferredSize(new Dimension(100,20));
        serialField.setVisible(false);

    }


    protected void setButtonBounds() {
        int top = (sizeH - Preferences.BUTTON_HEIGHT) / 2;
        int eachButton = Preferences.GUI_SMALL + Preferences.BUTTON_WIDTH;

        int cancelLeft = sizeW      - eachButton;
        int noLeft     = cancelLeft - eachButton;
        int yesLeft    = noLeft     - eachButton;

        yesButton.setSize(      Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
        noButton.setSize(       Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
        cancelButton.setSize(   Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
        okButton.setSize(       Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
        sendButton.setSize(     Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
        //serialRates.setSize(  3*Preferences.BUTTON_WIDTH/2, Preferences.BUTTON_HEIGHT);
        editField.setSize(    2*Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
        serialField.setSize(  3*Preferences.BUTTON_WIDTH, Preferences.BUTTON_HEIGHT);
        newlineChkBox.setSize((int)(1.5*Preferences.BUTTON_WIDTH), Preferences.BUTTON_HEIGHT);

        yesButton.setLocation(yesLeft, top);
        noButton.setLocation(noLeft, top);
        cancelButton.setLocation(cancelLeft, top);
        editField.setLocation(yesLeft - Preferences.BUTTON_WIDTH, top);
        serialField.setLocation(yesLeft - Preferences.BUTTON_WIDTH, top);
        okButton.setLocation(noLeft, top);
        serialRates.setLocation(5, top+2);
        newlineChkBox.setLocation(serialRates.getWidth() + 10, top);
        sendButton.setLocation(cancelLeft, top);

    }


    public Dimension getPreferredSize() {
        return getMinimumSize();
    }

    public Dimension getMinimumSize() {
        return new Dimension(300, Preferences.GRID_SIZE);
    }

    public Dimension getMaximumSize() {
        return new Dimension(3000, Preferences.GRID_SIZE);
    }


    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == noButton) {
            // shut everything down, clear status, and return
            unprompt();
            // don't need to save changes
            editor.checkModified2();

        } else if (e.getSource() == yesButton) {
            // answer was in response to "save changes?"
            unprompt();
            editor.handleSave(true);
            editor.checkModified2();

        } else if (e.getSource() == cancelButton) {
            // don't do anything, don't continue with checkModified2
            if (mode == PROMPT) unprompt();
            else if (mode == EDIT) unedit();
            editor.buttons.clear();

        } else if (e.getSource() == okButton) {
            // answering to "save as..." question
            String answer = editField.getText();
            if (!this.ignoreFileName) {
                editor.sketch.nameCode(answer);
            } else {
                this.ignoreFileName = true;
            }
            unedit();
        } else if (e.getSource() == sendButton) {
            editor.serialPort.write(serialField.getText());
            if (Preferences.getBoolean("serial.sendNewLine")) {
                editor.serialPort.write('\n');
            }
            serialField.setText("");
        } else if (e.getSource() == serialRates) {
            String wholeString = (String) serialRates.getSelectedItem();
            String rateString = wholeString.substring(0, wholeString.indexOf(' '));
            int rate = Integer.parseInt(rateString);
            Preferences.set("serial.debug_rate", rateString);
            editor.serialPort.dispose();
            try {
                editor.serialPort = new Serial(true);
            } catch (SerialException err) {
                editor.error(err);
            }
        }
    }



    /*
     * Generic method for creating a yes no message in the editor status screen.
     * */
    public void CreateYesNoDialog(final IYesNoListener listener, String message) {
        final EditorStatus editorStatus = this;
        if (listener != null) {
            this.yesButton.setVisible(true);
            this.noButton.setVisible(true);
            this.yesButton.removeActionListener(this);
            this.yesButton.addMouseListener(new MouseListener() {
                                                public void mouseClicked(MouseEvent arg0) {
                                                    listener.ResponseYes();
                                                    yesButton.addActionListener(editorStatus);
                                                    editorStatus.message = "";
                                                    yesButton.setVisible(false);
                                                    editorStatus.unprompt();
                                                }
                                                public void mouseEntered(MouseEvent arg0) {
                                                }
                                                public void mouseExited(MouseEvent arg0) {
                                                }
                                                public void mousePressed(MouseEvent arg0) {
                                                }

                                                public void mouseReleased(MouseEvent arg0) {
                                                }
                                            });

            this.noButton.removeActionListener(this);
            this.addMouseListener(new MouseListener() {
                                      public void mouseClicked(MouseEvent e) {
                                          listener.ResponseNo();
                                          noButton.addActionListener(editorStatus);
                                          noButton.setVisible(false);
                                      }

                                      public void mouseEntered(MouseEvent e) {
                                      }

                                      public void mouseExited(MouseEvent e) {
                                      }

                                      public void mousePressed(MouseEvent e) {
                                      }

                                      public void mouseReleased(MouseEvent e) {
                                      }
                                  });
            this.message = message;
            this.repaint();
        }
    }

    public void CreateOkDialog(final IOkListener listener, String message) {
        if (listener != null) {
            final EditorStatus es = this;
            this.message = message;
            this.okButton.setVisible(true);
            this.okButton.removeActionListener(this);
            this.okButton.addMouseListener(new MouseListener() {
                                               public void mouseClicked(MouseEvent arg0) {
                                                   System.out.println("launchedOkButtonEvent");
                                                   listener.OkButton();
                                                   okButton.setVisible(false);
                                                   okButton.addActionListener(es);
                                               }
                                               public void mouseEntered(MouseEvent arg0) {
                                               }
                                               public void mouseExited(MouseEvent arg0) {
                                               }
                                               public void mousePressed(MouseEvent arg0) {
                                               }
                                               public void mouseReleased(MouseEvent arg0) {
                                               }
                                           });
        }

    }

    boolean ignoreFileName = true;

    public void CreateOkEditDialog(final IOkListener listener, String message) {
        if (listener != null) {
            final EditorStatus es = this;
            this.message = message;
            this.okButton.setVisible(true);
            this.okButton.removeActionListener(this);
            this.edit(message, "default");
            ignoreFileName = false;
            this.editField.addKeyListener(new KeyListener() {
                                              public void keyPressed(KeyEvent arg0) {
                                                  if (arg0.getKeyCode() == KeyEvent.VK_ENTER) {
                                                      listener.OkButton();
                                                      okButton.setVisible(false);
                                                      okButton.addActionListener(es);
                                                      cancelButton.setVisible(false);
                                                      editField.setVisible(false);
                                                      editor.header.paintComponents(editor.getGraphics());
                                                      editField.removeKeyListener(this);
                                                  }
                                              }

                                              public void keyReleased(KeyEvent arg0) {
                                              }

                                              public void keyTyped(KeyEvent arg0) {
                                              }
                                          });
            this.okButton.addMouseListener(new MouseListener() {
                                               public void mouseClicked(MouseEvent arg0) {
                                                   listener.OkButton();
                                                   okButton.setVisible(false);
                                                   okButton.addActionListener(es);
                                                   cancelButton.setVisible(false);
                                                   editField.setVisible(false);
                                               }
                                               public void mouseEntered(MouseEvent arg0) {
                                               }
                                               public void mouseExited(MouseEvent arg0) {
                                               }
                                               public void mousePressed(MouseEvent arg0) {
                                               }
                                               public void mouseReleased(MouseEvent arg0) {
                                               }
                                           });
        }

    }


    private void CreateSerialDialog(final ISerialListener listener, String message) {
        if (listener != null) {
            this.message = message;
            this.okButton.setVisible(true);
            this.okButton.removeActionListener(this);
            this.okButton.addMouseListener(new MouseListener() {
                                               public void mouseClicked(MouseEvent e) {

                                               }
                                               public void mouseEntered(MouseEvent e) {
                                               }
                                               public void mouseExited(MouseEvent e) {
                                               }
                                               public void mousePressed(MouseEvent e) {
                                               }
                                               public void mouseReleased(MouseEvent e) {
                                               }

                                           });
        }
    }

}
