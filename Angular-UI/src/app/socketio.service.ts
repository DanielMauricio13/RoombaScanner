///<reference path="Constants.ts"/>


import {Injectable} from '@angular/core';
import io from 'socket.io-client'
import {Message} from "./models/message.model";
import {map} from "rxjs";






@Injectable({
  providedIn: 'root'
})
export class SocketioService {

private socket;

  constructor() {
    this.socket = io('127.0.0.1:5000', {
      reconnectionDelay: 1000,
      reconnection: true,
      reconnectionAttempts: 10,
      transports: ['websocket'],
      agent: false,
      rejectUnauthorized: false,
      path: '',
    });

    console.log('connecting');

    this.socket.on("connect_error", (err) => {
      console.log(`connect_error due to ${err.message}`);
    });
    this.socket.on("connection", (socket) => {
      console.log(socket);
    });

    this.socket.on("message", (data) => {
      this.receiveMessage(data);
    })
    this.socket.emit('message', "text");

  }

  receiveMessage(message: any){

  }


  sendMessage(message: Message){
    this.socket.emit('message', message);
  }





}
