///<reference path="../Constants.ts"/>

import {Drawable} from "./Drawable";
export class CyBot extends Drawable{

  private _angle: number; // angle in degrees

  constructor( xCm: number,  yCm: number) {
    super(xCm, yCm, 20);
    this.color = "#00AA00"
    this._angle = 0;
  }

  public move(cm:number){
    let deltaX = Math.cos(this._angle * Math.PI / 180) * cm;
    let deltaY = Math.sin(this._angle * Math.PI / 180) * cm;
    this.xCm = this.xCm + deltaX;
    this.yCm = this.yCm + deltaY;
    //TODO TEST
  }
  public turn(angle:number){
    let mAngle = this._angle + angle;
    //  Force angle to between +- 180
    if(mAngle > 180){
      mAngle = -1 * 360 - mAngle;
    }
    else if(mAngle < -180){
      mAngle = -1 * (-360 - 180);
    }
    this._angle = mAngle;
    //TODO TEST
  }

  get angle(){
    return this._angle;
  }

}
