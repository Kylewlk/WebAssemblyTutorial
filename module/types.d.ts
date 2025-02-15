

declare namespace Mym {
    declare class MyObject {
        constructor(n: number);
        printInfo(): void;

        createAsync(n: number): Promise<MyObject>;

        getN() : number;
    };

    function destroy(obj: any): void;
}

export = {Mym};