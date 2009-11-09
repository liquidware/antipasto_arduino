package antipasto.ModulePacker;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.swing.Box;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JWindow;

import antipasto.CoreFactory;
import antipasto.Interfaces.ICore;


public class ModuleGUI extends JPanel {
	JMenuBar menuBar;
	FilesList fileList;
	CoreList coreList;
	DetailsInputPanel inputPanel;
	
	public ModuleGUI(){
		super();
		this.setLayout(new BorderLayout());
		
		fileList = new FilesList(new Dimension(75, this.getHeight()));
		coreList = new CoreList();
		inputPanel = new DetailsInputPanel();
		
		JScrollPane pane = new JScrollPane(fileList);
		
		this.add(pane, BorderLayout.EAST);
		this.add(coreList, BorderLayout.WEST);
		this.add(inputPanel, BorderLayout.CENTER);
		
		pane.setVisible(true);
		fileList.setVisible(true);
		coreList.setVisible(true);
		inputPanel.setVisible(true);
		
		this.show();
		fileList.repaint();
		this.repaint();
	}
	
	
	
	/*
	 * Creates the GUI representation of the files that will be contained for a 
	 * module
	 * */
	class FilesList extends JList implements DropTargetListener{
		DropTarget dropTarget;
		ArrayList files = new ArrayList();
		
		public FilesList(Dimension dimension){
			super();
			this.setBackground(Color.black);
			dropTarget = new DropTarget(this, this);
			System.out.println("Constructing Files List");
		}
		
		public File[] getFiles(){
			File[] returnFiles = new File[files.size()];
			for(int i=0; i<files.size();i++){
				returnFiles[i] = (File) files.get(i);
			}
			return returnFiles;
		}

		public void dragEnter(DropTargetDragEvent arg0) {
			// TODO Auto-generated method stub
			
		}

		public void dragExit(DropTargetEvent arg0) {
			// TODO Auto-generated method stub
			
		}

		public void dragOver(DropTargetDragEvent arg0) {
			// TODO Auto-generated method stub
			
		}

		public void drop(DropTargetDropEvent arg0) {
			transferHandler(arg0.getTransferable());
		}

		public void dropActionChanged(DropTargetDragEvent arg0) {
			// TODO Auto-generated method stub
			
		}
		
		private void transferHandler(Transferable t){
			DataFlavor[] flavors = t.getTransferDataFlavors();
			if(t.isDataFlavorSupported(DataFlavor.javaFileListFlavor)){
				try {
					System.out.println(t.getTransferData(DataFlavor.javaFileListFlavor));
				} catch (UnsupportedFlavorException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}				
		}
		
		private void reloadItems(){
			this.removeAll();
			for(int i =0; i < files.size(); i++){
				this.add(new JListFileItem((File) this.files.get(i)));
			}
		}
		
		class JListFileItem extends JLabel{
			private File localFile;
			public JListFileItem(File f){
				super(f.getName());
				localFile = f;
			}
			
			public File getFile(){
				return localFile;
			}
		}
	}
	
	class CoreList extends JList implements DropTargetListener{
		ArrayList coreObjects = new ArrayList();
		public CoreList(){
			super();
		}

		public void dragEnter(DropTargetDragEvent arg0) {
			// TODO Auto-generated method stub
		}

		public void dragExit(DropTargetEvent arg0) {
			// TODO Auto-generated method stub
			
		}

		public void dragOver(DropTargetDragEvent arg0) {
			// TODO Auto-generated method stub
			
		}

		public void drop(DropTargetDropEvent arg0) {
			this.transferHandler(arg0.getTransferable());
		}

		public void dropActionChanged(DropTargetDragEvent arg0) {
			// TODO Auto-generated method stub
		}
		
		public void transferHandler(Transferable t){		
			DataFlavor[] flavors = t.getTransferDataFlavors();
			if(t.isDataFlavorSupported(DataFlavor.javaFileListFlavor)){
				try {
					File[] files = (File[]) t.getTransferData(DataFlavor.javaFileListFlavor);
					for(int i=0; i<files.length; i++){
						if(files[i].getName().toLowerCase().endsWith(".core")){
							CoreFactory fact = new CoreFactory();
							ICore core = fact.CreateCore(files[i], System.getProperty("java.io.tmpdir"));
						}
					}
				} catch (UnsupportedFlavorException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		
		private void reloadList(){
			this.removeAll();
			for(int i=0; i<this.coreObjects.size(); i++){
				ICore core = (ICore) this.coreObjects.get(i);
			}
		}
		
		class CoreListItem extends JLabel{
			private ICore containedCore;
			public CoreListItem(ICore core){
				super(core.getName());
				containedCore = core;
			}
			
			public ICore getCore(){
				return containedCore;
			}
		}
	}

	class DetailsInputPanel extends JPanel{
		JTextField nameInput;
		JComboBox majorVersion;
		JComboBox minorVersion;
		JComboBox revision;
		JTextField target;
		public DetailsInputPanel(){
			this.setSize(new Dimension(300, 300));
			this.initalizeForm();
		}
		
		private void initalizeForm(){
			Box vertBox = Box.createVerticalBox();
			vertBox.setSize(this.getSize());
			Box nameBox = Box.createHorizontalBox();
			Box versionBox = Box.createHorizontalBox();
			Box targetBox = Box.createHorizontalBox();

			JLabel nameLabel = new JLabel("Module Name: ");
			
			nameInput = new JTextField();
			nameInput.setSize(250, 10);
			
			JLabel targetLabel = new JLabel("Target: ");

			nameBox.add(nameLabel);
			nameBox.add(nameInput);
			
			
			majorVersion = this.createVersionComboBox();
			minorVersion = this.createVersionComboBox();
			revision = this.createVersionComboBox();
			
			target = new JTextField();
			
			versionBox.add(majorVersion);
			versionBox.add(minorVersion);
			versionBox.add(revision);
			targetBox.add(targetLabel);
			targetBox.add(target);
			
			vertBox.add(nameBox);
			vertBox.add(versionBox);
			vertBox.add(targetBox);
			
			nameLabel.setVisible(true);
			this.nameInput.setVisible(true);
			this.majorVersion.setVisible(true);
			this.minorVersion.setVisible(true);
			this.revision.setVisible(true);
			this.target.setVisible(true);
			
			this.add(vertBox);
			
			vertBox.setVisible(true);
			versionBox.setVisible(true);
			this.show();
		}
		
		private JComboBox createVersionComboBox(){
			JComboBox returnBox = new JComboBox();
			for(int i = 1; i<=9; i++){
				returnBox.addItem(new Integer(i));
			}
			return returnBox;
		}
	}
	
	public static void main(String[] args){
		JFrame wind = new JFrame();
		wind.setSize(new Dimension(350, 350));	
		wind.add(new ModuleGUI());
		wind.show();
	}
}


