import {useState} from "react";

export const MainSearch = () => {
    const [inputState, setInputState] = useState("")
    return <div
        className="w-full overflow-hidden h-fit bg-main2 mt-10 rounded-[30px] flex items-center justify-center font-medium text-[16px] xl:text-[18px]">
        <input type="text"
               className="basis-full bg-main border-none outline-none px-4 xl:px-8 leading-none h-12 xl:h-16 text-[black]"
               placeholder="Поиск Врачей, Клиник"
               onChange={e => setInputState(e.target.value)}/>
        <div className="w-fit h-fit leading-none font-bold text-[20px] flex shrink-0 px-4 xl:px-10  text-[white]">
            Искать
        </div>
    </div>
}