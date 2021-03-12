Js.log("Hello, BuckleScript and Reason!");

[@bs.deriving jsConverter]
type staticColor = [ | [@bs.as "white"] `white | [@bs.as "black"] `black];

[@bs.deriving jsConverter]
type themeStyleColor = [
  | [@bs.as "background"] `background
  | [@bs.as "foreground"] `foreground
];

type color = [ themeStyleColor | staticColor];

let colorFromJs = (str: string): option(color) => {
  let maybeThemeStyleColor = themeStyleColorFromJs(str);
  let maybeStaticColor = staticColorFromJs(str);
  switch (maybeThemeStyleColor, maybeStaticColor) {
  | (Some(tsc), None) =>
    switch (tsc) {
    | #themeStyleColor as c => Some(c)
    }
  | (None, Some(sc)) =>
    switch (sc) {
    | #staticColor as c => Some(c)
    }
  | (Some(_tsc), Some(_sc)) => None
  | (None, None) => None
  };
};

let _ = "white"->colorFromJs |> Js.log;
let waitForArray = () => Js.Promise.resolve([|1, 2, 3|]);
let source =
  Wonka.make((. observer) => {
    let next = observer.next;
    let complete = observer.complete;
  // let (next, complete) = observer;
    let cancelled = ref(false);
    let promise = waitForArray();
    let _ =
      Js.Promise.then_(
        arr =>
          ! cancelled^
            ? {
              Array.iter(next, arr);
              complete() |> Js.Promise.resolve;
            }
            : Js.Promise.resolve(),
        promise,
      );

    (.) => cancelled := true;
  });
// let waitForArray = () => Js.Promise.resolve([|1, 2, 3|]);
// let source =
//   Wonka.make((. observer) => {
//     let next = observer.next;
//     let complete = observer.complete;
//     let cancelled = ref(false);
//     let promise = waitForArray();
//    let _ = Js.Promise.then_(
//       arr =>
//         ! cancelled^ ? {
//           Array.iter(next, arr);
//           complete();
//         },
//       promise,
//     );
//     (.) => cancelled := true;
//   });
