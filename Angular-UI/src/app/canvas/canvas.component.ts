import {Component, ElementRef, OnInit, ViewChild} from '@angular/core';
import {animate} from "@angular/animations";
import {TallObstacle} from "../models/TallObstacle";
import {CyBot} from "../models/CyBot";
import {ScenarioService} from "../scenario.service";

@Component({
  selector: 'app-canvas',
  templateUrl: './canvas.component.html',
  styleUrls: ['./canvas.component.css']
})
export class CanvasComponent implements OnInit {



  @ViewChild('canvas', { static: true })
  canvas!: ElementRef<HTMLCanvasElement>;

  private ctx!: CanvasRenderingContext2D;


  constructor(private scenario: ScenarioService) {


  }

  ngOnInit(): void {
    this.ctx = this.canvas.nativeElement.getContext('2d')!;

    this.canvas.nativeElement.addEventListener("click", ev => {
        this.scenario.onClick(ev);
    });
  }

  animate(): void {
    this.scenario.drawElements(this.ctx);
  }




}
