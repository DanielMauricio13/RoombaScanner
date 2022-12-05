///<reference path="../Constants.ts"/>

import {Drawable} from "./Drawable";

export class CyBot extends Drawable{

  private _angle: number;

  constructor(xCm: number, yCm: number) {
    super(xCm, yCm, 20);
    this.color = "#00AA00"
    this._angle = 0;
  }

  public move(cm:number){
    //TODO
  }
  public turn(angle:number){
    //TODO update angle
  }

  get angle(){
    return this._angle;
  }
  set angle(angle){
    this._angle = angle;
  }

}
