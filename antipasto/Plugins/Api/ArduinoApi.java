package antipasto.Plugins.Api;

import processing.app.*;
import org.apache.log4j.Logger;

public class ArduinoApi {

    private Logger __logger = Logger.getLogger(ArduinoApi.class);

    public ArduinoApi() {
    }

    public String version() {
        return "v0";
    }

    //public void serialWrite(String s){
    //	Base.editor.serialPort.write(s);
    //}

    //public String serialRead(String s){
    //	String ret = Base.bl.bs.serialdata;
    //	Base.bl.bs.serialdata = "";
    //	return ret;
    //}
}
