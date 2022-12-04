import {Injectable} from '@angular/core';
import {Obstacle} from "./models/Obstacle";
import {CyBot} from "./models/CyBot";
import {SocketioService} from "./socketio.service";
import {scale} from "./Constants";

@Injectable({
  providedIn: 'root'
})
export class ScenarioService {


  private _obstacles: Obstacle[];
  private _cyBot!: CyBot;



  constructor(private socketService: SocketioService) {

    this._obstacles = [];
    this.cyBot = new CyBot(20, 20);
    this.socketService.sharedMessage.subscribe(value => {
      console.log(value);
    });


  }

  get cyBot(): CyBot {
    return this._cyBot;
  }

  set cyBot(value: CyBot) {
    this._cyBot = value;
  }

  get obstacles(): Obstacle[] {
    return this._obstacles;
  }


  addObstacle(obstacle: Obstacle) {
    this.obstacles.push(obstacle);
  }

  drawElements(ctx: CanvasRenderingContext2D){



    this.cyBot.draw(ctx);

    this.obstacles.forEach((obstacle) =>{
      obstacle.draw(ctx);
    });

  }
  onClick(ev:MouseEvent){
    let deltaX = ev.x - this._cyBot.getXCm;
    let deltaY = ev.y - this._cyBot.YCm;

    let newXCm = ev.x / scale;
    let newYCm = ev.y / scale;

    console.log("newXCm, newYCm", + newXCm+ " " + newYCm);

  }







}
