import {Injectable} from '@angular/core';
import {TallObstacle} from "./models/TallObstacle";
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
  private _cyBot!: CyBot;
  private _moving: boolean;
  updateGui: BehaviorSubject<boolean> = new BehaviorSubject<boolean>(false);


  constructor(private socketService: SocketioService) {

    this._tallObstacles = [];
    this._shortObstacles = [];

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

  }


  // MOUSE INTERACTIONS:

  /**
   * The user clicked a location on the screen
   * @param ev
   */
  onClick(ev:MouseEvent){
    if(!this._moving) {
      let deltaX = ev.x - this._cyBot.XCm;
      let deltaY = ev.y - this._cyBot.YCm;

      let newXCm = ev.y / scale;
      let newYCm = ev.x / scale;

      let mAngle = this._cyBot.angle;


      console.log("newXCm, newYCm", +newXCm + " " + newYCm);
      this.goToXYCM(newXCm, newYCm);
      this._moving = true;
    }
    else{
      console.log("CyBot is not ready to receive instructions!");
    }

  }

  goToXYCM(xCm: number, yCm: number){
    let deltaX = xCm - this._cyBot.XCm;
    let deltaY = yCm - this._cyBot.YCm;

    let cyBotAngle = this._cyBot.angle;

    let angleToCyBot = -1; //TODO MATH

    let distanceToCybot = -1; //TODO MATH


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

    console.log("Message: "  + msg);

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

    //TODO parse out angle, width
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
      // TODO add short obstacle
    }
    else if(msg == 'r'){
      // TODO add short obstacle
    }
    else if(msg == 'b'){
      // TODO add short obstacle
    }
    //TODO TEST
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

    if(msg == "ll"){
      // TODO plot line for cliff
    }
    else if(msg == "ml"){
      // TODO Plot line for cliff
    }
    else if(msg == "mr"){
      // TODO plot line for cliff
    }
    else if(msg == "rr"){
      // TODO plot line for cliff
    }

  }

  /**
   * CyBot is ready to receive more instructions
   * Called when message format "START READY END"
   * @param msg
   */
  getReadyMessage(msg: string){
    this._moving = true;
  }

  /**
   * CyBot sent a message that is not recognized by the code
   * @param msg
   */
  getUnknownMessage(msg: string){
    console.log("UNKNOWN MESSAGE: " + msg);
  }







}
