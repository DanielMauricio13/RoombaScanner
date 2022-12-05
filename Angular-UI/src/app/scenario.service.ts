import {Injectable} from '@angular/core';
import {TallObstacle} from "./models/TallObstacle";
import {CyBot} from "./models/CyBot";
import {SocketioService} from "./socketio.service";
import {scale} from "./Constants";

@Injectable({
  providedIn: 'root'
})
export class ScenarioService {


  private _tallObstacles: TallObstacle[];
  private _cyBot!: CyBot;
  private _moving: boolean;



  constructor(private socketService: SocketioService) {

    this._tallObstacles = [];
    this.cyBot = new CyBot(20, 20);
    this.socketService.sharedMessage.subscribe(value => {
      this.receiveMessage(value);
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



  drawElements(ctx: CanvasRenderingContext2D){



    this.cyBot.draw(ctx);

    this.tallObstacles.forEach((obstacle) =>{
      obstacle.draw(ctx);
    });

  }
  onClick(ev:MouseEvent){
    if(!this._moving) {
      let deltaX = ev.x - this._cyBot.getXCm;
      let deltaY = ev.y - this._cyBot.YCm;

      let newXCm = ev.x / scale;
      let newYCm = ev.y / scale;

      console.log("newXCm, newYCm", +newXCm + " " + newYCm);
      this._moving = true;
    }

  }
  receiveMessage(msg: string){
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
    else if(msg.startsWith("amount moved")){
      this.getAmountMoved(msg);
    }
    else if(msg.startsWith("amount turned")){
      this.getAmountTurned(msg);
    }

  }
  getTallObstacleMessage(msg: string){
    // START obj <angle>, <distance>, <width> END

    msg = msg.replace("obj", "");
    msg = msg.trim();

    //TODO parse out angle, width
    console.log("Received Object (angle, distance):" + msg);
    //TODO TEST
  }
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

  getAmountMoved(msg: string){
    msg = msg.replace("amount moved", "");
    msg = msg.trim();

    var dist = Number(msg.replace(/[^0-9\.]+/g,""));
    this._cyBot.move(dist);

    this._moving = false;
    //TODO TEST
  }
  getAmountTurned(msg: string){
    msg = msg.replace("amount turned", "");
    msg = msg.trim();
    var angle = Number(msg.replace(/[^0-9\.]+/g,""));

    this._cyBot.turn(angle);
    //TODO TEST

  }
  getCliffMessage(msg: string){
    msg = msg.replace("cilff", "");
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
  getUnknownMessage(msg: string){
    console.log("UNKNOWN MESSAGE: " + msg);
  }







}
