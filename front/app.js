const BASE = 'http://localhost:8080/irc';

const $ = id => document.getElementById(id);
const logEl = $('log');
let es = null;

function log(kind, msg, data) {
  const time = new Date().toISOString();
  const pre = document.createElement('div');
  pre.style.marginBottom = '6px';
  pre.innerHTML = `<strong>[${time}] ${kind}</strong>: ${msg}` + (data ? '<pre style="white-space:pre-wrap">' + JSON.stringify(data, null, 2) + '</pre>' : '');
  logEl.prepend(pre);
}

async function post(path, body) {
  const res = await fetch(BASE + path, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body)
  });
  let text = null;
  try { text = await res.text(); } catch(e){}
  let json = null;
  try { json = text ? JSON.parse(text) : null } catch(e) { json = text }
  log('POST ' + path, `status=${res.status}`, json);
  return { status: res.status, body: json };
}

async function get(path) {
  const res = await fetch(BASE + path, { method: 'GET' });
  const json = await res.json().catch(() => null);
  log('GET ' + path, `status=${res.status}`, json);
  return { status: res.status, body: json };
}

async function connect() {
  const body = {
    host: $('host').value,
    port: parseInt($('port').value, 10),
    password: $('password').value,
    nickname: $('nickname').value,
    realName: $('realName').value
  };
  await post('/connect', body);
}

async function connectMessage() {
  const body = {
    host: $('host').value,
    port: parseInt($('port').value, 10),
    password: $('password').value,
    nickname: $('nickname').value,
    realName: $('realName').value,
    channel: $('channel').value,
    message: $('message').value
  };
  await post('/connect-message', body);
}

async function join() {
  const body = { channel: $('channel').value };
  await post('/join', body);
}

async function message() {
  const body = { channel: $('channel').value, message: $('message').value };
  await post('/message', body);
}

async function disconnect() {
  await post('/disconnect', {});
}

async function status() {
  await get('/status');
}

async function history() {
  await get('/history');
}

function subscribe() {
  if (es) { log('SSE', 'already subscribed'); return }
  es = new EventSource(BASE + '/events');
  es.onmessage = e => {
    // default message
    log('SSE message', e.data);
  };
  es.addEventListener('irc-event', e => { log('SSE irc-event', e.data); });
  es.addEventListener('history', e => { log('SSE history', e.data); });
  es.onerror = e => { log('SSE', 'error', e); es.close(); es = null };
  log('SSE', 'subscribed');
}

function unsubscribe() {
  if (!es) { log('SSE', 'not subscribed'); return }
  es.close(); es = null; log('SSE', 'unsubscribed');
}

// bind buttons
$('btnConnect').addEventListener('click', connect);
$('btnConnectMsg').addEventListener('click', connectMessage);
$('btnStatus').addEventListener('click', status);
$('btnDisconnect').addEventListener('click', disconnect);
$('btnJoin').addEventListener('click', join);
$('btnMessage').addEventListener('click', message);
$('btnHistory').addEventListener('click', history);
$('btnSubscribe').addEventListener('click', subscribe);
$('btnUnsubscribe').addEventListener('click', unsubscribe);

log('info', 'Frontend loaded. Open this file from a simple http server and use the controls.');
