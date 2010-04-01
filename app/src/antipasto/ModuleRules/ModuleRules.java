package antipasto.ModuleRules;

import java.util.ArrayList;

/*
 * Entity class for maintaining all the rules for a module
 * @Author electricFeel
 * */
public class ModuleRules {
	IMessage[] _messages;
	
	public IMessage[] getMessages() {
		return _messages;
	}
	
	void setMessages(ArrayList messages){
		IMessage[] messageArr = new IMessage[messages.size()];
		for(int i=0; i < messages.size(); i++){
			messageArr[i] = (IMessage)messages.get(i);
		}
		_messages = messageArr;
	}
}
