package antipasto.GUI.GadgetListView;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;
import java.awt.geom.RoundRectangle2D;
import java.io.File;
import java.io.FilenameFilter;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.EventListenerList;

import processing.app.Base;
import processing.app.Sketchbook;

import antipasto.ScriptRunner;
import antipasto.GUI.ImageListView.ScriptCellRenderer;
import antipasto.Interfaces.*;
import antipasto.Util.Utils;

public class WingPanelScripts extends JPanel {
	
	boolean isUpdated = false;
	String text;
	Font font;
	
	private Color bgDefaultColor =Color.WHITE;
	private Color bgHoverColor = Color.WHITE;
	private Color bgColor;
	
	private Color textDefaultColor = Color.BLACK;
	private Color textHoverColor = Color.WHITE;
	private Color textColor;
	
	private double width = 300; 
	private double height = 450;
	private boolean isFocused = false;
	private boolean isSelected = false;
	
	private String userdir = System.getProperty("user.dir") + File.separator;
	private String jrubyPath = new String(userdir + "hardware"+File.separator+
													"tools"+File.separator+
													"jruby"+File.separator+
													"bin"+File.separator);
	private String scriptPath = new String(Sketchbook.getSketchbookPath() + 
											File.separator); 
	private JPanel     headerPanel;
	private JLabel     headerLabel;
	private JList 	    scriptFileList;
	private JScrollPane scrollPane;
	private WingFooter  wingFooter;
	
	/**
	 * Creates a new Wing Panel for scripts
	 * @param The directory where the scripts a located and executed from.
	 * @param wingFooter The panel to display status to the user
	 * @param width The width of the panel
	 * @param height The height of the panel
	 */
	public WingPanelScripts(String scriptDirectory,
							Object wingFooter, 
							int width, int height){

		this.width = width;
		this.height = height;
		
		/* Setup the panel */
		setSize(width, height);
		setBackground(bgDefaultColor);
		setLayout(new BorderLayout());
		
		headerPanel = initScriptPanelHeader(" | Run | ");
		
		setDirectory(scriptDirectory); 
		
		this.wingFooter = (WingFooter)wingFooter;
		
		/* Add */
		add(headerPanel, BorderLayout.NORTH);
		add(scrollPane, BorderLayout.CENTER);
	}
	
	/**
	 * Initialize a Script Panel Header
	 * @param textDisplay The button text
	 * @return: a JPanel with a button
	 */
	JPanel initScriptPanelHeader(String textDisplay) {
		
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
					wingFooter.setText(" Run " + selectedItem + " script");
				} else {
					wingFooter.setText(" Run script");
				}
				
			}

			public void mouseExited(MouseEvent arg0) {
				headerLabel.setForeground(Color.white);
			}

			public void mousePressed(MouseEvent arg0) {
				
				String selectedItem = (String) scriptFileList.getSelectedValue();
				if (selectedItem != null) {
					wingFooter.setText(" Running " + selectedItem + " script");
				} else {
					wingFooter.setText(" Run script");
				}
			}

			/* JRuby Script Execution
			 */
			public void mouseReleased(MouseEvent arg0) {
				
				/* Run the selected script */
				executeJRubyScript((String) scriptFileList.getSelectedValue());

			}});
		
		headerLabel.setForeground(new Color(0xFF, 0xFF, 0xFF));
		headerPanel.add(headerLabel, BorderLayout.CENTER);		
		
		return headerPanel;
	}
	
	/**
	 * Initialize Script Body 
	 * @param scriptList A list of files from the scriptDirectory
	 */
	void initScriptBody(String[] scriptList) {
		
		scriptFileList = new JList(scriptList);
		scriptFileList.setCellRenderer(new ScriptCellRenderer());
		
		scrollPane = new JScrollPane(scriptFileList);
		scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED);
		scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		scrollPane.setVisible(true);

	}
	
	/**
	 *  Execute a JRuby Script
	 * If the script name is not null, this executes it.
	 */
	void executeJRubyScript(String scriptName) {
		

		if (scriptName != null) {
			
			ScriptRunner myScript = new ScriptRunner(jrubyPath + "jruby.bat", 
			      	 								 jrubyPath + "jruby");

			try {
				myScript.run(scriptPath + scriptName);
				System.out.println(myScript.getScriptOutput());
				((WingFooter)wingFooter).setText(" Script finished.");
			} catch (Exception e) {
				Base.showWarning("JRuby Error", "Could not find the JRuby Compiler\n", null);
				e.printStackTrace();			
			}
			
		} else {
			/* Display the issue */
			wingFooter.setText("Select a script to run.");
		}
	}
	
	/**
	 * 
	 * @param Sets the directory that scripts are read and executed from.
	 */
	public void setDirectory(String scriptDirectory) {
		scriptPath = scriptDirectory;
		
		initScriptBody(readScripts());
	}
	
	/**
	 * 
	 * @return script files from the script directory
	 */
	private String[] readScripts() {
		
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
		
		return fileList;
	}
	
}
