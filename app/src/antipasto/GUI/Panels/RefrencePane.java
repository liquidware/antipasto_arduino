package antipasto.GUI.Panels;

import java.net.URL;

import javax.swing.JScrollPane;
import javax.swing.JTextPane;

public class RefrencePane extends JScrollPane{
	
	private JTextPane textPane;
	
	public RefrencePane(String htmlPage){
		textPane = new JTextPane();
		this.getViewport().add(textPane);
		
		try{
			URL url = new URL(htmlPage);
			textPane.setPage(url);
		}catch(Exception ex){
			System.out.println(ex.getMessage());
		}
	}
	
	public void changeRefrence(String htmlPage){
		try{
			URL url = new URL(htmlPage);
			textPane.setPage(url);
		}catch(Exception ex){
			System.out.println(ex.getMessage());
		}
	}
}
