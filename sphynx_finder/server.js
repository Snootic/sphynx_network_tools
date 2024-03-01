import express from 'express'
import scanNetwork from "./deviceFinder.js";
import cors from 'cors';

var app = express();
app.use(cors());

app.get("/", async function (req, res) {
    let scan = await scanNetwork();
    res.json((scan))
});

app.listen(3000, function () {
  console.log("App de Exemplo escutando na porta 3000!");
});
