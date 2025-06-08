import {useState} from "react";
import { useRouter } from 'next/navigation';

export const MainSearch = () => {
    const [inputState, setInputState] = useState("")
    const router = useRouter();

    const handleSearch = () => {
        if (inputState.trim()) {
            // Перенаправляем на страницу поиска с поисковым запросом
            router.push(`/search?query=${encodeURIComponent(inputState.trim())}`);
        }
    }

    const handleKeyPress = (e) => {
        if (e.key === 'Enter') {
            handleSearch();
        }
    }

    return <div
        className="w-full overflow-hidden h-fit bg-main2 mt-10 rounded-[30px] flex items-center justify-center font-medium text-[16px] xl:text-[18px]">
        <input type="text"
               className="basis-full bg-main border-none outline-none px-4 xl:px-8 leading-none h-12 xl:h-16 text-[black]"
               placeholder="Поиск Врачей, Клиник"
               value={inputState}
               onChange={e => setInputState(e.target.value)}
               onKeyPress={handleKeyPress}/>
        <div
            className="w-fit h-fit leading-none font-bold text-[20px] flex shrink-0 px-4 xl:px-10 text-[white] cursor-pointer hover:bg-opacity-90"
            onClick={handleSearch}>
            Искать
        </div>
    </div>
}

