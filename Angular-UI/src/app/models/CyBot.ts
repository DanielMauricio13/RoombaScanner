///<reference path="../Constants.ts"/>

import {Drawable} from "./Drawable";

export class CyBot extends Drawable{


  constructor(xCm: number, yCm: number) {
    super(xCm, yCm, 20);
    this.color = "#FF0000"
  }
}
