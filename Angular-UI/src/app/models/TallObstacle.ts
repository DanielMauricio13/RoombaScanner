import { Component, OnInit } from '@angular/core';
import {fieldWidth} from "../Constants";
import {Drawable} from "./Drawable";
///<reference path="../Constants.ts"/>

export class TallObstacle extends Drawable{



  constructor(xCm: number, yCm: number, width: number) {
    super(xCm, yCm, width);
    this.color = "#0000FF";
  }
}
