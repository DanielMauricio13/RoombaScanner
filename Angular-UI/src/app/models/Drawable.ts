import {scale} from "../Constants";

export class Drawable {
  protected color: string = "3370d4"


  constructor(protected xCm: number, protected yCm: number, protected width: number) {
  }

  /**
   * Draw the drawable. Note we use the y axis to represent the x axis for the bot for math purposes
   * @param ctx
   */
  draw(ctx: CanvasRenderingContext2D) {

    ctx.fillStyle = this.color;
    ctx.arc(this.yCm * scale, this.xCm * scale, this.width * scale, 0, 2 * Math.PI);
    ctx.fill();
    ctx.stroke();
  }

   setX(_x: number){
    this.xCm = _x
  }
  setY(_y: number){
    this.yCm = _y;
  }
  get XCm(){
    return this.xCm;
  }
  get YCm(){
    return this.yCm;
  }
}
