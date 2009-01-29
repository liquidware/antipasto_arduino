package antipasto.GUI.ImageListView;

import java.awt.BorderLayout;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JButton;
import javax.swing.JFrame;

import antipasto.GUI.GadgetListView.GadgetPanel;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.ActiveGadgetObject;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveGadgetChangedEventListener;
import antipasto.Interfaces.IModule;

import processing.app.Serial;
import processing.app.SerialException;
import processing.app.TouchShieldImageTransfer;


public class ImageListPanel extends JFrame implements IActiveGadgetChangedEventListener {
	private ImageListView list;
	private TouchShieldImageTransfer imageTransfer;
	private JButton transferButton;
	private GadgetPanel panel;
	public ImageListPanel(GadgetPanel panel, TouchShieldImageTransfer imageTransfer){
		this.imageTransfer = imageTransfer;
		list = new ImageListView(panel);
		this.getContentPane().setLayout(new BorderLayout());
		this.panel = panel;
		this.getContentPane().add(list, BorderLayout.CENTER);
		this.createTransferButton();
		this.getContentPane().add(transferButton, BorderLayout.SOUTH);
		panel.addActiveGadgetChangedEventListener(this);
		this.doVisabilityCheck();
	}
	
	private void createTransferButton(){
		this.transferButton = new JButton("Transfer");
		this.transferButton.addMouseListener(new MouseListener(){

			public void mouseClicked(MouseEvent arg0) {
				transfer();
			}

			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			public void mousePressed(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}

			public void mouseReleased(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			
		});
	}
	
	private void transfer(){
		try {
			this.imageTransfer.setSerial(new Serial(true));
			this.imageTransfer.ModuleImageTransfer(panel.getActiveModule());
		} catch (SerialException e) {
			e.printStackTrace();
		}
	}

	public void onActiveGadgetChanged(ActiveGadgetObject obj) {
		this.doVisabilityCheck();
	}
	
	private void doVisabilityCheck(){
		IModule module = panel.getActiveModule();
		boolean hasImage = false;
		if(module != null){
			for(int i = 0; i < panel.getActiveModule().getData().length; i++){
				if(module.getData()[i].getName().endsWith(".bmp")){
					hasImage = true;
					break;
				}
			}
		}else{
			this.hide();
		}
		if(hasImage){
			this.show();
		}else{
			this.hide();
		}
	}
}
