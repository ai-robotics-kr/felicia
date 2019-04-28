import feliciaJs from 'felicia_js.node';
import TYPES from 'common/connection-type';
import MasterProxyClient from './master-proxy-client';
import websocket from './websocket';
import { isWin } from './lib/environment';

export default () => {
  const ws = websocket();
  feliciaJs.MasterProxy.setBackground();

  if (isWin) {
    global.MasterProxyClient = MasterProxyClient;
    feliciaJs.MasterProxy.startGrpcMasterClient();
  }

  const s = feliciaJs.MasterProxy.start();
  if (!s.ok()) {
    process.exit(1);
  }

  function requestRegisterDynamicSubscribingNode() {
    if (isWin) {
      if (!feliciaJs.MasterProxy.isClientInfoSet()) {
        setTimeout(requestRegisterDynamicSubscribingNode, 1000);
        return;
      }
    }

    feliciaJs.MasterProxy.requestRegisterDynamicSubscribingNode(
      (topic, message) => {
        console.log(`[TOPIC]: ${topic}`);
        if (message.type === 'felicia.CameraMessage') {
          ws.broadcast(message.message.data, TYPES.Camera);
        } else {
          ws.broadcast(
            JSON.stringify({
              type: message.type,
              data: message.message,
            }),
            TYPES.General
          );
        }
      },
      (topic, status) => {
        console.log(`[TOPIC]: ${topic}`);
        console.error(status.errorMessage());
      },
      {
        period: 100,
        queue_size: 1,
      }
    );
  }

  requestRegisterDynamicSubscribingNode();

  feliciaJs.MasterProxy.run();
};