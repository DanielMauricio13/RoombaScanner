import { Component, OnInit } from '@angular/core';
import {fieldWidth} from "../Constants";
import {Drawable} from "./Drawable";
///<reference path="../Constants.ts"/>

export class ShortObstacle extends Drawable{



  constructor(xCm: number, yCm: number) {
    super(xCm, yCm, 13);
    this.color = "#000088";
  }
}
