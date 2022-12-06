import {Injectable} from '@angular/core';
import {TallObstacle} from "./models/TallObstacle";
import {Cliff} from './models/Cliff';
import {CyBot} from "./models/CyBot";
import {SocketioService} from "./socketio.service";
import {scale} from "./Constants";
import {ShortObstacle} from "./models/ShortObstacle";
import {BehaviorSubject} from "rxjs";

@Injectable({
  providedIn: 'root'
})
export class ScenarioService {


  private _tallObstacles: TallObstacle[];
  private _shortObstacles: ShortObstacle[];
  private _cliffs: Cliff[];
  private _cyBot!: CyBot;
  private _moving: boolean;
  updateGui: BehaviorSubject<boolean> = new BehaviorSubject<boolean>(false);


  constructor(private socketService: SocketioService) {

    this._tallObstacles = [];
    this._shortObstacles = [];
    this._cliffs = [];

    this.cyBot = new CyBot(20, 20);
    this.socketService.sharedMessage.subscribe(value => {
      this.receiveMessage(value);
      this.updateGui.next(true);

    });
    this._moving = false;
  }

  get cyBot(): CyBot {
    return this._cyBot;
  }

  set cyBot(value: CyBot) {
    this._cyBot = value;
  }

  get tallObstacles(): TallObstacle[] {
    return this._tallObstacles;
  }

  addTallObstacle(obstacle: TallObstacle) {
    this.tallObstacles.push(obstacle);
  }

  get shortObstacles(): ShortObstacle[]{
    return this._shortObstacles;
  }
  addShortObstacle(xCm:number, yCm:number){
    let obstacle = new ShortObstacle(xCm, yCm);
    this._shortObstacles.push(obstacle);
  }

  get cliffs(): Cliff[] {
    return this._cliffs;
  }
  addCliff(xCm:number, yCm:number){
    this._cliffs.push(new Cliff(xCm, yCm));
  }

  /**
   * Draw all drawable elements on the canvas
   * @param ctx
   */
  drawElements(ctx: CanvasRenderingContext2D){



    this.cyBot.draw(ctx);

    this.tallObstacles.forEach((obstacle) =>{
      obstacle.draw(ctx);
    });

    this.shortObstacles.forEach((obstacle) =>{
      obstacle.draw(ctx);
    });

    this.cliffs.forEach((cliff) =>{
      cliff.draw(ctx);
    });

  }


  // MOUSE INTERACTIONS:

  /**
   * The user clicked a location on the screen
   * @param ev
   */
  onClick(ev:MouseEvent){
    console.log(" " + ev.offsetX + " " + ev.offsetY);


      let newXCm = (ev.x - 10) / scale;
      let newYCm = (ev.y - 75.5) / scale;

      let mAngle = this._cyBot.angle;

      console.log("Clicked " + newXCm + ", " + newYCm);

      this.goToXYCM(newXCm, newYCm);



  }

  goToXYCM(xCm: number, yCm: number){
    let cyX = this._cyBot.XCm;
    let cyY = this._cyBot.YCm;

    let xChange = xCm - cyX;
    let yChange = yCm - cyY;

    let angleChange = Math.atan2(yChange , xChange);

    let distance = Math.sqrt(xChange * xChange + yChange * yChange) - 20;

    let cyBotAngle = this._cyBot.angle;

    let newAngle = (cyBotAngle + (angleChange * 180 / Math.PI)) % 360;

    console.log("Found: " + angleChange + "xchange, ychange" + xChange + " "  + yChange);
    console.log("Found: angle: " + newAngle + ", distance " + distance);
    this.socketService.sendMessage("" + parseInt(String(newAngle))+ ","+ parseInt(String(distance)) + "q" +"\r\n");

    this._moving = true;

  }

  //MESSAGES
  /**
   * Receive general message (called from subscription) over socket
   * @param msg
   */
  receiveMessage(msg: string){
    msg = msg.trim();
    msg = msg.replace("START", "");
    msg = msg.replace("END", "");
    msg = msg.trim();


    if(msg.startsWith("obj")){
      this.getTallObstacleMessage(msg);
    }
    else if(msg.startsWith("bump")){
      this.getBumpMessage(msg);
    }
    else if(msg.startsWith("cliff")){
      this.getCliffMessage(msg);
    }
    else if(msg.startsWith("move")){
      this.getAmountMoved(msg);
    }
    else if(msg.startsWith("turn")){
      this.getAmountTurned(msg);
    }
    else if(msg.startsWith("READY")){
      this.getReadyMessage(msg);
    }


  }

  /**
   * Found a tall obstacle using IR sensor
   * Called when message of format "START obj <distance> END"
   * @param msg
   */
  getTallObstacleMessage(msg: string){
    // START obj <angle>, <distance>, <width> END

    msg = msg.replace("obj", "");
    msg = msg.trim();
    let textTemp1= msg.substring(0, msg.indexOf(','));
    let angle= parseInt(textTemp1);
    msg= msg.replace(textTemp1+ ',', "");
    textTemp1= msg.substring(0, msg.indexOf(','));
    let distance= parseInt(textTemp1);
    msg= msg.replace(textTemp1+ ',', "");
    let width= parseInt(msg);
    let tempX;
    let tempY;
    if(angle >0){
      tempY= Math.sin(angle) * distance;
      tempY=this._cyBot.YCm + tempY;
      let c= (distance * distance) - ( tempY * tempY);
      c= Math.sqrt(c);
      if(angle > 90)
        tempX= this._cyBot.YCm - c;
      else
        tempX= this._cyBot.YCm + c;

    }
    else{
      angle= angle * -1;
      tempY= Math.sin(angle) * distance;
      tempY=this._cyBot.YCm - tempY;
      let c= (distance * distance) - ( tempY * tempY);
      c= Math.sqrt(c);
      if(angle > -90)
        tempX= this._cyBot.YCm + c;
      else
        tempX= this._cyBot.YCm - c;

    }
    this._tallObstacles.push(new TallObstacle(tempX,tempY,width))
    console.log("Received Object (angle, distance):" + msg);
    //TODO TEST
  }

  /**
   * Robot bumped into an obstacle
   * Called whe message of format "START BUMP {l/r/b} END"
   * @param msg
   */
  getBumpMessage(msg: string){
    msg = msg.replace("bump", "");
    msg = msg.trim();
   if(msg == 'l'){
        if( this._cyBot.angle > 45 && this._cyBot.angle <= 135)
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm + 8, this._cyBot.YCm +8));
        else if(this.cyBot.angle > -135 &&  this._cyBot.angle < -45)
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm -8, this.cyBot.YCm -8));
        else if((this._cyBot.angle > 0 && this._cyBot.angle < 45) || (this._cyBot.angle < -135))
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm +8, this.cyBot.YCm -8));
        else if(this._cyBot.angle > 135 || (this._cyBot.angle < 0 && this._cyBot.angle >= -45))
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm -8 , this.cyBot.YCm +8));

    }
    else if(msg == 'r'){
        if( this._cyBot.angle > 45 && this._cyBot.angle <= 135)
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm - 8, this._cyBot.YCm +8));
        else if(this.cyBot.angle > -135 &&  this._cyBot.angle < -45)
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm +8, this.cyBot.YCm -8));
        else if((this._cyBot.angle > 0 && this._cyBot.angle < 45) || (this._cyBot.angle < -135))
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm +8, this.cyBot.YCm +8));
        else if(this._cyBot.angle > 135 || (this._cyBot.angle < 0 && this._cyBot.angle >= -45))
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm -8 , this.cyBot.YCm -8));
    }
    else if(msg == 'b'){
        if( this._cyBot.angle > 45 && this._cyBot.angle <= 135)
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm , this._cyBot.YCm -8));
        else if(this.cyBot.angle > -135 &&  this._cyBot.angle < -45)
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm , this.cyBot.YCm +8));
        else if((this._cyBot.angle > 0 && this._cyBot.angle < 45) || (this._cyBot.angle < -135))
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm - 8, this.cyBot.YCm ));
        else if(this._cyBot.angle > 135 || (this._cyBot.angle < 0 && this._cyBot.angle >= -45))
          this._shortObstacles.push(new ShortObstacle(this._cyBot.XCm +8 , this.cyBot.YCm ));
    }
    else{
      this.getUnknownMessage("bump " +msg);
    }
  }
  /**
   * Robot drove a certain distance from current position
   * Called wHen message of format "START amount moved <distance> END
   * @param msg
   */
  getAmountMoved(msg: string){
    msg = msg.replace("move", "");
    msg = msg.trim();
    console.log("Moved: " + msg);

    var dist = Number(msg.replace(/[^0-9\.]+/g,""));
    this._cyBot.move(dist);

    //TODO TEST
  }

  /**
   * CyBot turned a certain angle from current position
   * Called when message format "START amount turned <angle> END"
   * @param msg
   */
  getAmountTurned(msg: string){
    msg = msg.replace("turn", "");
    msg = msg.trim();
    console.log("Turned: " + msg);
    var angle = Number(msg.replace(/[^0-9\.]+/g,""));

    this._cyBot.turn(angle);
    //TODO TEST

  }

  /**
   * CyBot detected a cliff at the corresponding sensor
   * Called when message format "START cliff {ll/ml/mr/rr} END"
   * @param msg
   */
  getCliffMessage(msg: string){
    msg = msg.replace("cliff", "");
    msg = msg.trim();

    let cyX = this._cyBot.XCm;
    let cyY = this._cyBot.YCm;

    let clX: number;
    let clY: number;

    let cyBotAngle = this._cyBot.angle;

    if(msg == "ll"){
      clX=cyX+40*Math.sin(((cyBotAngle-70)*Math.PI)/180);
      clY=cyY-40*Math.cos(((cyBotAngle-70)*Math.PI)/180);
    }
    else if(msg == "ml"){
      clX=cyX+40*Math.sin(((cyBotAngle-25)*Math.PI)/180);
      clY=cyY-40*Math.cos(((cyBotAngle-25)*Math.PI)/180);
    }
    else if(msg == "mr"){
      clX=cyX+40*Math.sin(((cyBotAngle+25)*Math.PI)/180);
      clY=cyY-40*Math.cos(((cyBotAngle+25)*Math.PI)/180);
    }
    else if(msg == "rr"){
      clX=cyX+40*Math.sin(((cyBotAngle+70)*Math.PI)/180);
      clY=cyY-40*Math.cos(((cyBotAngle+70)*Math.PI)/180);
    }
    else{
      clX=-1;
      clY=-1;
    }
    this._cliffs.push(new Cliff(clX,clY));
    console.log("Detected Cliff at Sensor:" + msg);

  }

  /**
   * CyBot is ready to receive more instructions
   * Called when message format "START READY END"
   * @param msg
   */
  getReadyMessage(msg: string){
    console.log("READY RECEIVED");
    this._moving = false;
  }

  /**
   * CyBot sent a message that is not recognized by the code
   * @param msg
   */
  getUnknownMessage(msg: string){
    console.log("UNKNOWN MESSAGE: " + msg);
  }








}
