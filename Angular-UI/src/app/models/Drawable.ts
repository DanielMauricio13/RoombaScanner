import {fieldWidth} from "../Constants";

export class Drawable {
  protected color: string = "3370d4"


  constructor(protected xCm: number, protected yCm: number, protected width: number) {
  }

  draw(ctx: CanvasRenderingContext2D) {
    let width = ctx.canvas.clientWidth;
    let height = ctx.canvas.clientHeight;

    let scale: number = width / fieldWidth;

    console.log(this.xCm * scale);


    ctx.fillStyle = this.color;
    ctx.arc(this.xCm * scale, this.yCm * scale, this.width * scale, 0, 2 * Math.PI);
    ctx.fill();
    ctx.stroke();
  }
}
