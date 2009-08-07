package antipasto.Plugins;

import antipasto.Plugins.Interfaces.*;
import org.java.plugin.Plugin;
import java.util.EventListener;
import java.util.EventObject;


public abstract class PluginBase extends Plugin implements EditorListener{

	protected void init()
	{
		
	}
	
	protected void postInit()
	{
		System.out.println("POST INIT");
	}

	protected void applicationVisible()
	{
	      
	}
	
	public void getApplication()
	{

	} 

	public void setContext()
	{

	} 
	
	public void stop()
	{

	} 

	public void handleEvent(EventObject event){
		System.out.println("HANDLED:"+event.toString());
	}
}
