import {Drawable} from "./Drawable";
import {scale} from "../Constants";
///<reference path="../Constants.ts"/>

export class Cliff extends Drawable{



  constructor(xCm: number, yCm: number) {
    super(xCm, yCm, 13);
    this.color = "#000088";
  }

  override draw(ctx: CanvasRenderingContext2D) {

    ctx.font = "15px calibri";
    ctx.strokeText("x", this.xCm, this.yCm);
    
    ctx.fillStyle = this.color;

    ctx.fill();
    ctx.stroke();
  }
}
