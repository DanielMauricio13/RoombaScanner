///<reference path="Constants.ts"/>


import {Injectable} from '@angular/core';
import io from 'socket.io-client'
import {Message} from "./models/message.model";
import {BehaviorSubject, map, Observable, ReplaySubject, Subject} from "rxjs";






@Injectable({
  providedIn: 'root'
})
export class SocketioService {





private _socket;
sharedMessage = new ReplaySubject<string>();

  constructor() {
    this._socket = io('127.0.0.1:5000', {
      reconnectionDelay: 1000,
      reconnection: true,
      reconnectionAttempts: 10,
      transports: ['websocket'],
      agent: false,
      rejectUnauthorized: false,
      path: '',
    });

    console.log('connecting');

    this._socket.on("connect_error", (err) => {
      console.log(`connect_error due to ${err.message}`);
    });
    this._socket.on("connection", (socket) => {
      console.log(socket);
    });
    this.socket.on("message", (msg) =>{
      this.sharedMessage.next(msg);
      }
    );

  }


  sendMessage(message: string){
    this._socket.emit('message', message);
  }

  get socket() {
    return this._socket;
  }




}
