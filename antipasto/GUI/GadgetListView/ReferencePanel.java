package antipasto.GUI.GadgetListView;

import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.ContainerEvent;
import java.awt.event.ContainerListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.font.FontRenderContext;
import java.awt.geom.Line2D;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.List;

import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.ScrollPaneLayout;

import antipasto.ScriptRunner;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.ActiveGadgetObject;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveGadgetChangedEventListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveSketchChangingListener;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.SketchChangingObject;
import antipasto.GUI.ImageListView.ScriptCellRenderer;
import antipasto.Interfaces.IModule;
import antipasto.Util.GadgetFileFilter;
import antipasto.Util.ScriptFileFilter;
import antipasto.Util.Utils;
import processing.app.*;

public class ReferencePanel extends JDialog implements ComponentListener,
		IActiveGadgetChangedEventListener, FocusListener {

	// The standard width and height for the dialog
	private int cachedHeight = 425;
	private int cachedWidth = 300;
	private JTextArea textArea;
	private JLabel statusLabel;
	private JLabel headerLabel;
	private JFrame component;
	private IModule activeModule;
	private JScrollPane scrollPane;
	private JList scriptFileList;
	
	private String userdir = System.getProperty("user.dir") + File.separator;
	private String jrubyPath = new String(userdir + "hardware"+File.separator+
													"tools"+File.separator+
													"jruby"+File.separator+
													"bin"+File.separator);
	
	private String scriptPath = new String(Sketchbook.getSketchbookPath() + 
									   		File.separator); 
	
	private String headerTextDefault = new String("                  " +
			   									  "                   | Run |");
	private String[] wingTabNames;
	
	private WingTab[] wingTabs;
	private JPanel[] wingDocs;
	
	public ReferencePanel(JFrame parent) {
		super(parent, false);
		component = parent;
		this.setUndecorated(true);
		this.init();
		parent.addComponentListener(this);
		this.addComponentListener(this);
	}

	public void LoadText(String txt) {
		this.textArea.setText(txt);
	}
	
	/*************************************************
	 * Initialize Top Panel
	 *  Returns: a JPanel with the Tabs for the right wing.
	 */
	JPanel initTopPanel(String[] tabNames) {

		JPanel bgPanel = new JPanel();
		bgPanel.setSize(cachedWidth, 15);
		bgPanel.setBackground(new Color(0x21, 0x68, 0x86));
		bgPanel.setLayout(new BorderLayout());
		
		/* Create the tab Panel */
		JPanel tabPanel = new JPanel();
		tabPanel.setBackground(new Color(0x21, 0x68, 0x86));
		tabPanel.setSize(cachedWidth, 15);
		tabPanel.setLayout(new BoxLayout(tabPanel,BoxLayout.X_AXIS));
		tabPanel.setOpaque(true);
		
		/* Create the tabs */
		wingTabNames = tabNames.clone();
		for (int x=0; x < wingTabNames.length; x++) {
			WingTab tab = new WingTab(wingTabNames[x],null);
			tabPanel.add(tab);
		}
		tabPanel.addMouseListener(new MouseListener() {

			@Override
			public void mouseClicked(MouseEvent e) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseEntered(MouseEvent e) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseExited(MouseEvent e) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mousePressed(MouseEvent e) {
				// TODO Auto-generated method stub
				
			}

			@Override
			public void mouseReleased(MouseEvent e) {
				// TODO Auto-generated method stub
				//this.
			}
			
		});
		
		/* A Horizontal Separator */
		JComponent hrImg = new JComponent() {
									private static final long serialVersionUID = 1L;

									public void paint(Graphics g) {
							 	        Graphics2D graphics2 = (Graphics2D) g;
							 	        graphics2.setBackground(new Color(255,255,255));
							 	        graphics2.setColor(new Color(255,255,255));
							 	        graphics2.drawLine(0,0,700,1);
							       	 }};
		
		bgPanel.add(tabPanel, BorderLayout.NORTH);					       	 
		bgPanel.add(hrImg, BorderLayout.CENTER);	
		
		return bgPanel;
	}

	/*************************************************
	 * Initialize Bottom Panel
	 *  Returns: a JPanel with the status panel for the right wing.
	 */
	JPanel initBottomPanel(String message) {

		JPanel bottomPanel = new JPanel();
		bottomPanel.setSize(cachedWidth, 15);
		bottomPanel.setBackground(new Color(0x04, 0x4F, 0x6F));
		bottomPanel.setLayout(new BorderLayout());

		statusLabel = new JLabel(message);
		statusLabel.setForeground(new Color(0xFF, 0xFF, 0xFF));
		bottomPanel.add(statusLabel, BorderLayout.WEST);

		return bottomPanel;
	}
	
	/* Execute a JRuby Script
	 * If the script name is not null, this executes it.
	 */
	void executeScript(String scriptName) {
		

		if (scriptName != null) {
			
			ScriptRunner myScript = new ScriptRunner(jrubyPath + "jruby.bat", 
			      	 								 jrubyPath + "jruby");

			try {
				myScript.run(scriptPath + scriptName);
				System.out.println(myScript.getScriptOutput());
				statusLabel.setText(" Script finished.");
			} catch (Exception e) {
				Base.showWarning("JRuby Error", "Could not find the JRuby Compiler\n", null);
				e.printStackTrace();			
			}
			
		} else {
			/* Display the issue */
			statusLabel.setText("Select a script to run.");
		}
	}
	
	/*************************************************
	 * Initialize Tab Header Panel
	 *  Returns: a JPanel with a header text 
	 */
	JPanel initTabHeader(String textDisplay) {
		
		JPanel headerPanel = new JPanel();
		headerPanel.setBackground(new Color(0x04, 0x4F, 0x6F));
		headerPanel.setLayout(new BorderLayout());

		headerLabel = new JLabel(textDisplay);
		
		headerLabel.addMouseListener(new MouseListener() {

			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				headerLabel.setForeground(Color.orange);
				
				// TODO Auto-generated method stub
				String selectedItem = (String) scriptFileList.getSelectedValue();
				if (selectedItem != null) {
					statusLabel.setText(" Run " + selectedItem + " script");
				} else {
					statusLabel.setText(" Run script");
				}
				
			}

			public void mouseExited(MouseEvent arg0) {
				headerLabel.setText(headerTextDefault);
				headerLabel.setForeground(Color.white);
			}

			public void mousePressed(MouseEvent arg0) {
				
				String selectedItem = (String) scriptFileList.getSelectedValue();
				if (selectedItem != null) {
					statusLabel.setText(" Running " + selectedItem + " script");
				} else {
					statusLabel.setText(" Run script");
				}
			}

			/* JRuby Script Execution Testing
			 */
			public void mouseReleased(MouseEvent arg0) {
				
				executeScript((String) scriptFileList.getSelectedValue());

			}});
		
		headerLabel.setForeground(new Color(0xFF, 0xFF, 0xFF));
		headerPanel.add(headerLabel, BorderLayout.CENTER);		
		
		return headerPanel;
	}
	
	/*************************************************
	 * Initialize Reference Tab 
	 *  Returns: a JPanel with the reference content 
	 */
	JPanel initReferenceTab(String defaultText) {

		JPanel panel = new JPanel();
		panel.setLayout(new BorderLayout());
		
		this.getContentPane().setLayout(new BorderLayout());
		this.textArea = new JTextArea(defaultText);

		this.setSize(cachedWidth, cachedHeight);
		this.setBackground(new Color(0x04, 0x4F, 0x6F));

		this.scrollPane = new JScrollPane(this.textArea);
		this.scrollPane
				.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		this.scrollPane
				.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);

		this.textArea.setWrapStyleWord(true);
		this.textArea.setLineWrap(true);
		this.textArea.setBounds(this.scrollPane.getBounds());

		this.textArea.setVisible(true);
		this.scrollPane.setVisible(true);

		this.textArea.addFocusListener(this);

		//Build the header
		JPanel headerPanel = initTabHeader(" Module: TouchShield.txt ");
		
		// Assemble the panel
		panel.add(headerPanel, BorderLayout.NORTH);
		panel.add(this.scrollPane,BorderLayout.CENTER);

		return panel;		
		
		
		
	}
	
	/*************************************************
	 * Initialize Script Tab 
	 *  Returns: a JPanel with the script content 
	 */
	JPanel initScriptTab(String[] scriptList) {

		JPanel panel = new JPanel();
		panel.setLayout(new BorderLayout());
		
		scriptFileList = new JList(scriptList);
		scriptFileList.setCellRenderer(new ScriptCellRenderer());
		
		this.scrollPane = new JScrollPane(scriptFileList);
		this.scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		this.scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		this.scrollPane.setVisible(true);
		
		JPanel headerPanel = initTabHeader(headerTextDefault);
		
		// Assemble the panel
		panel.add(headerPanel, BorderLayout.NORTH);
		panel.add(this.scrollPane,BorderLayout.CENTER);
		
		return panel;

	}

	/*************************************************
	 * Initialize the Center Panel
	 * 
	 * The Center Panel always returns:  --------------
	 * 								      Header Panel
	 * 						     	      Content Panel
	 *								     ---------------
	 */ 								
	JPanel initCenterPanel(String tab) {
		
		//Find some files from the sketchbook/scripts directory
		// For testing, lets create some default files
		
		// Grab the script files from the sketchbook directory
		File scriptFolder = new File(scriptPath);
		String fileList[] = scriptFolder.list(new FilenameFilter() {

			public boolean accept(File dir, String name) {
		        String extension = Utils.getExtension(name);
		        if (extension != null) {
		            if (extension.equals(Utils.rb) ){
		                    return true;
		            } 
		        }
		        
				return false;
			} });
		
		
		  // Build and Return the script tab panel
		  return initScriptTab(fileList);
		 
		//return initReferenceTab("Here is some default text   \n" +
		//					    "from the TouchShield module \n" +
		//					    "Line 3                      \n" +
		//					    "Line 4                      \n");
	}

	/*************************************************
	 * Initialize the Right Wing
	 * 
	 * The Right Wing always has:   --------------
	 * 								  Tab Panel
	 * 						     	  Content Panel
	 * 							      Status Panel
	 * 								---------------
	 */
	private void init() {

		this.getContentPane().setLayout(new BorderLayout());
		this.setSize(cachedWidth, cachedHeight);
		this.setBackground(new Color(0x04, 0x4F, 0x6F));
		
		JPanel topPanel    = initTopPanel(new String[] {" Reference ", " Scripts ", " Wiring ", " Plugin "});
		JPanel centerPanel = initCenterPanel("Scripts");
		JPanel bottomPanel = initBottomPanel(" Scripts loaded.");

		this.getContentPane().add(centerPanel, BorderLayout.CENTER);
		this.getContentPane().add(topPanel, BorderLayout.NORTH);
		this.getContentPane().add(bottomPanel, BorderLayout.SOUTH);

		this.scrollPane.setVisible(true);
	}

	
	private void setLocation() {
		if (this.isVisible() && ((Editor) component).centerPanel.isVisible()) {
			int xLocation = component.getX() + component.getWidth();
			int yLocation = ((Editor) component).centerPanel
					.getLocationOnScreen().y;
			this.setLocation(xLocation, yLocation);
		}
	}

	// Component Listener: This is designed to listen specifically to the editor
	// windows so that we can adjust our size according to
	// it for symmetry...specifically with the gadget panel
	public void componentHidden(ComponentEvent arg0) {

	}

	public void componentMoved(ComponentEvent arg0) {
		setLocation();
	}

	public void componentResized(ComponentEvent arg0) {
		// TODO Auto-generated method stub

	}

	public void componentShown(ComponentEvent arg0) {
		setLocation();
		repaint();
	}

	public void onActiveGadgetChanged(ActiveGadgetObject obj) {
		if (obj != null) {
			System.out
					.println("Active gadget chainging in the reference panel");
			this.activeModule = obj.getModule();
			if (this.activeModule.getReferenceText() != null
					|| this.activeModule.getReferenceText()
							.equalsIgnoreCase("")) {
				this.LoadText(this.activeModule.getReferenceText());
			} else {
				System.out.println("No text found");
				this.textArea.setText("");
			}
		} else {
			this.textArea.setText("");
			System.out.println("Setting text to blank");
		}
	}

	public void focusGained(FocusEvent arg0) {

	}

	public void focusLost(FocusEvent arg0) {
		if (this.activeModule != null) {
			this.activeModule.setReferenceText(this.textArea.getText());
			System.out.println("setting the text");
		} else {
			System.out.println("Text being set to null");
		}
	}
	
	public void message(String s) {
		System.out.println(s);
	}
	

}
