import {scale} from "../Constants";

export class Drawable {
  protected color: string = "3370d4"


  constructor(protected xCm: number, protected yCm: number, protected width: number) {
  }

  draw(ctx: CanvasRenderingContext2D) {

    ctx.fillStyle = this.color;
    ctx.arc(this.xCm * scale, this.yCm * scale, this.width * scale, 0, 2 * Math.PI);
    ctx.fill();
    ctx.stroke();
  }

   setX(_x: number){
    this.xCm = _x
  }
  setY(_y: number){
    this.yCm = _y;
  }
  get getXCm(){
    return this.xCm;
  }
  get YCm(){
    return this.yCm;
  }
}
